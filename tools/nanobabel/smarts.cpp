
#include "nanobabel.h"

using namespace OpenBabel;

class SmartsContext
{
  public:
    std::string data_dir;
    std::string file_input;
    std::string file_output;
    std::string smarts_pattern;
    bool hydrogens;
    bool unique;
};

void computeSmarts(SmartsContext context)
{
  /*
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
    error("SMARTS Filtered molecule is empty");
  }
  // Optionally add hydrogens
  if (context.hydrogens)
  {
    log("Adding hydrogens");
    mol.AddHydrogens();
  }
  // Apply SMARTS filter
  OBSmartsPattern sp;
  sp.Init(context.smarts_pattern);
  sp.Match(mol);
  vector<vector<int> > maplist;
  if (context.unique)
  {
    maplist = sp.GetUMapList();
  }
  else
  {
    maplist = sp.GetMapList();
  }
  // Read SMARTS filter results
  log("Smarts results");
  vector<vector<int> >::iterator i;
  vector<int>::iterator j;
  for (i = maplist.begin(); i != maplist.end(); ++i)
  {
    for (j = i->begin(); j != i->end(); ++j)
    {
      std::cout << j << " ";
    }
    std::cout << endl;
  }
  // Build resulting OBMol
  log("Smarts filtered");
  OBMol rmol;
  rmol.Clear();
  rmol.BeginModify();
  for (i = maplist.begin(); i != maplist.end(); ++i)
  {
    for (j = i->begin(); j != i->end(); ++j)
    {
      OBAtom *matched = mol.GetAtom(j);
      rmol.AddAtom(*matched);
    }
  }
  rmol.EndModify();
  // Write result
  log("Writing results");
  std::string output_str = conv_out.WriteString(&rmol);
  writeFile(context.file_output, output_str);
  log("Wrote output file: " + context.file_output + " (" + toString(output_str.length() / 1024) + "kB)");
  log("Exiting");
  */
}

void runSmarts(int argc, char **argv)
{
  // Init context
  SmartsContext context;
  context.data_dir = "";
  context.file_input = "input.pdb";
  context.file_output = "output.pdb";
  context.smarts_pattern = "*";
  context.hydrogens = false;
  context.unique = false;
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
    else if ((option == "-s" || option == "--smarts") && (argc > (i + 1)))
    {
      context.smarts_pattern = toString(argv[i + 1]);
      i++;
    }
    else if (option == "-h" || option == "--hydrogen" || option == "--hydrogens")
    {
      context.hydrogens = true;
    }
    else if (option == "-u" || option == "--unique" || option == "--no-duplicate")
    {
      context.unique = true;
    }
    else
    {
      error("Unknown option: " + option);
    }
  }
  // Run Smarts
  computeSmarts(context);
}
