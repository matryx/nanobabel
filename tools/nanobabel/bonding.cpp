
#include "nanobabel.h"

using namespace OpenBabel;

class BondingContext
{
  public:
    std::string data_dir;
    std::string file_input;
    std::string file_output;
    bool hydrogens;
};

void bondingSetup(BondingContext context)
{
  // Setting up data dir
  log("Setup environment");
  if (context.data_dir.length() > 0)
  {
    setenv("BABEL_DATADIR", context.data_dir.c_str(), 1);
  }
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
  // I/O
  log("Preparing I/O");
  std::string input_str = readFile(context.file_input);
  log("Read input file: " + context.file_input + " (" + toString(input_str.length() / 1024) + "kB)");
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
    error("Bonded molecule is empty");
  }
  // Optionally add hydrogens
  if (context.hydrogens)
  {
    log("Adding hydrogens");
    mol.AddHydrogens();
  }
  mol.Kekulize();
  //debugBonds(&mol);
  // Write result
  std::string output_str = conv_out.WriteString(&mol);
  writeFile(context.file_output, output_str);
  log("Wrote output file: " + context.file_output + " (" + toString(output_str.length() / 1024) + "kB)");
  log("Exiting");
}

void runBonding(int argc, char **argv)
{
  // Init context
  BondingContext context;
  context.data_dir = "";
  context.file_input = "input.pdb";
  context.file_output = "output.pdb";
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
    else if (option == "-o" && (argc > (i + 1)))
    {
      context.file_output = toString(argv[i + 1]);
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
  // Run bonding
  bondingSetup(context);
}
