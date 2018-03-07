
#include "nanobabel.h"

using namespace OpenBabel;

class MinimizeContext
{
  public:
    std::string data_dir;
    std::string file_input;
    std::string file_output;
    std::string file_ctx;
    std::string ffid;
    int logSteps;
    int steps;
    bool hydrogens;
    bool steepestDescent;
    bool logEnergy;
};

void minimizeSetup(MinimizeContext context)
{
  // Setting up data dir
  log("Setup environment");
  if (context.data_dir.length() > 0)
  {
    setenv("BABEL_DATADIR", context.data_dir.c_str(), 1);
  }
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
    error("Input format not supported" + toString(format_in->GetID()));
  }
  log("Using input format: " + toString(format_in->GetID()));
  // Check output format
  if (!format_out)
  {
    error("Unrecognized output format: " + context.file_output);
  }
  if (!conv_out.SetOutFormat(format_out))
  {
    error("Output format not supported" + toString(format_out->GetID()));
  }
  log("Using output format: " + toString(format_out->GetID()));
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
  // Optionally add hydrogens
  if (context.hydrogens)
  {
    log("Adding hydrogens");
    mol.AddHydrogens();
  }
  // Setup constraints
  OBFFConstraints constraints;
  if (context.file_ctx != "")
  {
    log("Setting up constraints");
    if (!applyConstraints(&mol, &constraints, ctx_str))
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
    // Update progress in console
    if (i % 10 == 0)
    {
      log("Optimization progress: " + toString(i * 100 / context.steps) + "%," + " " + toString(pFF->Energy()) + "E");
    }
    i++;
    // Apply gradients
    if (context.steepestDescent)
    {
      doing = pFF->SteepestDescentTakeNSteps(1);
    }
    else
    {
      doing = pFF->ConjugateGradientsTakeNSteps(1);
    }
    // Exit if exploded
    if (pFF->DetectExplosion())
    {
      std::string output_str = conv_out.WriteString(&mol);
      writeFile(context.file_output, output_str);
      log("Wrote output file: " + context.file_output + " (" + toString(output_str.length() / 1024) + "kB)");
      error("Explosion has occured!");
    }
    // Save for next round
    pFF->GetCoordinates(mol);
    // Write structure update on specified interval
    if (context.logSteps > 0 && i % context.logSteps == 0)
    {
      std::string step_output_str = conv_out.WriteString(&mol);
      log("Step update start: " + toString(i));
      std::cout << std::endl << step_output_str << std::endl;
      log("Step update end: " + toString(i));
      if (context.logEnergy)
      {
        double energy = pFF->Energy();
        log("Energy update: " + toString(energy));
      }
    }
  }
  log("Optimization progress: " + toString(i * 100 / context.steps) + "%" + " " + toString(pFF->Energy()));
  // Write result
  log("Minimization done");
  pFF->GetCoordinates(mol);
  std::string output_str = conv_out.WriteString(&mol);
  writeFile(context.file_output, output_str);
  log("Wrote output file: " + context.file_output + " (" + toString(output_str.length() / 1024) + "kB)");
  log("Exiting");
}

void runMinimize(int argc, char **argv)
{
  // Init context
  MinimizeContext context;
  context.data_dir = "";
  context.steps = 2500;
  context.logSteps = -1;
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
      context.file_input = toString(argv[i + 1]);
      i++;
    }
    else if (option == "-o" && (argc > (i + 1)))
    {
      context.file_output = toString(argv[i + 1]);
      i++;
    }
    else if (option == "-cx" && (argc > (i + 1)))
    {
      context.file_ctx = toString(argv[i + 1]);
      i++;
    }
    else if (option == "-ff" && (argc > (i + 1)))
    {
      context.ffid = toString(argv[i + 1]);
      i++;
    }
    else if (option == "-n" && (argc > (i + 1)))
    {
      context.steps = atoi(argv[i + 1]);
      i++;
    }
    else if (option == "-l" && (argc > (i + 1)))
    {
      context.logSteps = atoi(argv[i + 1]);
      i++;
    }
    else if (option == "-dd" && (argc > (i + 1)))
    {
      context.data_dir = toString(argv[i + 1]);
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
    else if (option == "-e")
    {
      context.logEnergy = true;
    }
    else
    {
      error("Unknown option: " + option);
    }
  }
  // Run minimization
  minimizeSetup(context);
}