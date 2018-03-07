
#include "nanobabel.h"

using namespace OpenBabel;

class EnergyContext
{
  public:
    std::string data_dir;
    std::string file_input;
    std::string ffid;
    bool hydrogens;
};

void energySetup(EnergyContext context)
{
  // Setting up data dir
  log("Setup environment");
  if (context.data_dir.length() > 0)
  {
    setenv("BABEL_DATADIR", context.data_dir.c_str(), 1);
  }
  log("Energy starting");
  // Find filetype
  OBConversion conv_in;
  log("Detecting I/O formats");
  OBFormat *format_in = conv_in.FormatFromExt(context.file_input);
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
  // Energy setup
  log("Setup Energy context");
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
  log("Energy ready");
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
    error("Energy rated molecule is empty");
  }
  // Optionally add hydrogens
  if (context.hydrogens)
  {
    log("Adding hydrogens");
    mol.AddHydrogens();
  }
  // Setup forcefield
  log("Setup forcefield");
  if (!pFF->Setup(mol))
  {
    error("Could not setup force field.");
  }
  // Read energy
  log("Read energy");
  double energy = pFF->Energy();
  log("Energy result: " + toString(energy));
  // Write result
  log("Energy done");
  log("Exiting");
}

void runEnergy(int argc, char **argv)
{
  // Init context
  EnergyContext context;
  context.data_dir = "";
  context.ffid = "UFF";
  context.file_input = "input.pdb";
  context.hydrogens = false;
  // Parse arguments
  for (int i = 2; i < argc; i++)
  {
    std::string option(argv[i]);
    if (option == "-i" && (argc > (i + 1)))
    {
      context.file_input = toString(argv[i + 1]);
      i++;
    }
    else if (option == "-ff" && (argc > (i + 1)))
    {
      context.ffid = toString(argv[i + 1]);
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
    else
    {
      error("Unknown option: " + option);
    }
  }
  // Run Energy
  energySetup(context);
}