
#include "nanobabel.h"

using namespace OpenBabel;

class MinimizationContext
{
  public:
    std::string file_input;
    std::string file_output;
    std::string file_ctx;
    std::string ffid;
    int steps;
    bool hydrogens;
    bool steepestDescent;
};

std::string constrainMinimizationResidueKey(std::string chainName, std::string residueName, int residueSerial)
{
  return chainName + ":" + residueName + ":" + toString(residueSerial);
}

bool constrainMinimization(OBMol *mol, OBFFConstraints *constraints, std::string content)
{
  // Read locked residues
  std::map<std::string, bool> residuesLockeds;
  std::vector<std::string> lines = split(content, '\n');
  for (int i = 0; i < lines.size(); i++)
  {
    std::string line = lines[i];
    std::vector<std::string> parts = split(line, ':');
    if (parts.size() >= 1)
    {
      std::string type = parts[0];
      if (type == "RESIDUE" && parts.size() >= 2)
      {
        std::string constraint = parts[1];
        if (constraint == "FIXED" && parts.size() == 5)
        {
          std::string chainName = parts[2];
          std::string residueName = parts[3];
          int residueSerial = atoi(parts[4].c_str());
          std::string residueKey = constrainMinimizationResidueKey(chainName, residueName, residueSerial);
          residuesLockeds[residueKey] = true;
          //log("Locking key: " + residueKey);
        }
      }
    }
  }
  // Loop over which atoms should be locked
  int constrained = 0;
  // constraints->SetFactor(10);
  for (OBAtomIterator aitr = mol->BeginAtoms(), aend = mol->EndAtoms(); aitr != aend; ++aitr)
  {
    OBAtom *atom = *aitr;
    if (atom)
    {
      OBResidue *residue = atom->GetResidue();
      if (residue)
      {
        std::string chainName = std::string(1, residue->GetChain());
        std::string residueName = std::string(residue->GetName());
        int residueSerial = residue->GetNum();
        std::string residueKey = constrainMinimizationResidueKey(chainName, residueName, residueSerial);
        //log("Testing key: " + residueKey);
        if (residuesLockeds.count(residueKey) > 0)
        {
          constraints->AddAtomConstraint(atom->GetIdx());
          constrained += 1;
        }
      }
    }
  }
  // Done
  log("Constrained: " + toString(constrained) + " of " + toString(mol->NumAtoms()) + " atoms");
  return true;
}

void minimizationSetup(MinimizationContext context)
{
  log("Minimization starting");
  // Find filetype
  OBConversion conv_in;
  OBConversion conv_out;
  log("Detecting I/O formats");
  OBFormat *format_in = conv_in.FormatFromExt(context.file_input);
  OBFormat *format_out = conv_out.FormatFromExt(context.file_output);
  // Check input format
  if (!format_in)
  {
    error("Unrecognized input format: " + context.file_input);
  }
  if (!conv_in.SetInFormat(format_in))
  {
    error("Input format not supported" + std::string(format_in->GetMIMEType()));
  }
  log("Using input format: " + std::string(format_in->GetMIMEType()));
  // Check output format
  if (!format_out)
  {
    error("Unrecognized output format: " + context.file_output);
  }
  if (!conv_out.SetOutFormat(format_out))
  {
    error("Output format not supported" + std::string(format_out->GetMIMEType()));
  }
  log("Using output format: " + std::string(format_out->GetMIMEType()));
  // Find forcefield
  log("Loading forcefield: " + context.ffid);
  OBForceField* pFF = OBForceField::FindForceField(context.ffid);
  if (!pFF)
  {
    error("Could not find forcefield: " + context.ffid);
  }
  // I/O
  log("Preparing I/O");
  std::string input_str = readFile(context.file_input);
  log("Read input file: " + context.file_input + " (" + toString(input_str.length() / 1024) + "kB)");
  std::string ctx_str;
  if (context.file_ctx != "")
  {
    ctx_str = readFile(context.file_ctx);
    log("Read contraints file: " + context.file_ctx + " (" + toString(ctx_str.length() / 1024) + "kB)");
  }
  else
  {
    log("No constraint file specified");
  }
  //std::string ctx_str = readFile(context.file_ctx);
  //log("CtxFile: " + toString(ctx_str.length()));
  // Minimization setup
  log("Setup Minimization context");
  double crit = 1e-6;
  bool sd = true;
  double rvdw = 6.0;
  double rele = 10.0;
  bool newton = false;
  int freq = 10;
  bool cut = false;
  pFF->SetLogFile(&std::cerr);
  pFF->SetLogLevel(OBFF_LOGLVL_NONE);
  pFF->SetVDWCutOff(rvdw);
  pFF->SetElectrostaticCutOff(rele);
  pFF->SetUpdateFrequency(freq);
  pFF->EnableCutOff(cut);
  if (newton)
  {
    pFF->SetLineSearchType(LineSearchType::Newton2Num);
  }
  log("Minimization ready");
  // Load mol
  OBMol mol;
  mol.Clear();
  log("Parse input molecule: " + context.file_input);
  if (!conv_in.ReadString(&mol, input_str))
  {
    error("Failed to read input file: " + context.file_input);
  }
  log("Checking");
  if (mol.Empty())
  {
    error("Miminized molecule is empty");
  }
  debugResidues(&mol);
  debugAtoms(&mol);
  debugBonds(&mol);
  // Optionally add hydrogens
  if (context.hydrogens)
  {
    log("Adding hydrogens");
    mol.AddHydrogens();
  }

  debugResidues(&mol);
  debugAtoms(&mol);
  debugBonds(&mol);

  // Setup constraints
  OBFFConstraints constraints;
  if (context.file_ctx != "")
  {
    log("Setting up constraints");
    if (!constrainMinimization(&mol, &constraints, ctx_str))
    {
      error("Could not setup constraints");
    }
  }
  // Setup forcefield
  log("Setup forcefield");
    if (!pFF->Setup(mol, constraints))
  {
    error("Could not setup force field.");
  }
  // Gradients setup
  log("Start gradient optiminization");
  if (context.steepestDescent)
  {
    pFF->SteepestDescentInitialize(context.steps, crit);
  }
  else
  {
    pFF->ConjugateGradientsInitialize(context.steps, crit);
  }
  // Loop to get better gradients
  bool doing = true;
  int i = 0;
  while (doing)
  {
    if (i % 10 == 0)
    {
      log("Optimization progress: " + toString(i * 100 / context.steps) + "%" + " " + toString(pFF->Energy()));
    }
    i++;
    if (context.steepestDescent)
    {
      doing = pFF->SteepestDescentTakeNSteps(1);
    }
    else
    {
      doing = pFF->ConjugateGradientsTakeNSteps(1);
    }
    if (pFF->DetectExplosion())
    {
      std::string output_str = conv_out.WriteString(&mol);
      log("Wrote output file: " + context.file_output + " (" + toString(output_str.length() / 1024) + "kB)");
      writeFile(context.file_output, output_str);
      error("Explosion has occured!");
    }
    else
    {
      pFF->GetCoordinates(mol);
    }
  }
  log("Optimization progress: " + toString(i * 100 / context.steps) + "%" + " " + toString(pFF->Energy()));
  // Write result
  log("Minimization done");
  pFF->GetCoordinates(mol);
  std::string output_str = conv_out.WriteString(&mol);
  log("Wrote output file: " + context.file_output + " (" + toString(output_str.length() / 1024) + "kB)");
  writeFile(context.file_output, output_str);
  log("Exiting");
}

void runMinimization(int argc, char **argv)
{
  // Init context
  MinimizationContext context;
  context.steps = 2500;
  context.ffid = "UFF";
  context.file_input = "input.pdb";
  context.file_output = "output.pdb";
  context.file_ctx = "";
  context.hydrogens = false;
  context.steepestDescent = false;
  // Parse arguments
  for (int i = 2; i < argc; i++)
  {
    std::string option(argv[i]);
    if (option == "-i" && (argc > (i + 1)))
    {
      context.file_input = std::string(argv[i + 1]);
      i++;
    }
    else if (option == "-o" && (argc > (i + 1)))
    {
      context.file_output = std::string(argv[i + 1]);
      i++;
    }
    else if (option == "-cx" && (argc > (i + 1)))
    {
      context.file_ctx = std::string(argv[i + 1]);
      i++;
    }
    else if (option == "-ff" && (argc > (i + 1)))
    {
      context.ffid = std::string(argv[i + 1]);
      i++;
    }
    else if (option == "-n" && (argc > (i + 1)))
    {
      context.steps = atoi(argv[i + 1]);
      i++;
    }
    else if (option == "-h")
    {
      context.hydrogens = true;
    }
    else if (option == "-sd")
    {
      context.steepestDescent = true;
    }
    else
    {
      error("Unknown option: " + option);
    }
  }
  // Run minimization
  minimizationSetup(context);
}