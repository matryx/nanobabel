
#include "nanobabel.h"

void usage()
{
  std::cout << "Usage: nanobabel [action] [options]" << std::endl;
  std::cout << std::endl;
  std::cout << "  all actions:" << std::endl;
  std::cout << "    minimize        Molecular energy minimization" << std::endl;
  std::cout << "    bonding         Compute bonding information of molecule" << std::endl;
  std::cout << "    convert         Rewrite molecule file in a different format" << std::endl;
  std::cout << "    hydrogen        Add or remove hydrogens from a molecule file" << std::endl;
  std::cout << "    forcefields     List supported forcefields" << std::endl;
  std::cout << "    formats         List supported file formats" << std::endl;
  std::cout << "    descriptors     List supported descriptors" << std::endl;
  std::cout << "    plugins         List supported plugins" << std::endl;
  std::cout << "    config          Current configuration informations" << std::endl;
  std::cout << std::endl;
  std::cout << "  minimize options:" << std::endl;
  std::cout << "    -i   FILE       Input file (default=input.pdb)" << std::endl;
  std::cout << "    -o   FILE       Output file (default=output.pdb)" << std::endl;
  std::cout << "    -cx  FILE       Forcefield constraints file" << std::endl;
  std::cout << "    -ff  FFID       Select a forcefield (default=UFF)" << std::endl;
  std::cout << "    -h              Add hydrogen atoms" << std::endl;
  std::cout << "    -n   steps      Specify the maximum number of steps (default=2500)" << std::endl;
  std::cout << "    -l   steps      Specify the interval number of steps between structure updates" << std::endl;
  std::cout << "    -dd  PATH       Set the path for the data directory" << std::endl;
  std::cout << std::endl;
  std::cout << "  bonding options:" << std::endl;
  std::cout << "    -i   FILE       Input file (default=input.pdb)" << std::endl;
  std::cout << "    -o   FILE       Output file (default=output.pdb)" << std::endl;
  std::cout << "    -h              Add hydrogen atoms" << std::endl;
  std::cout << "    -dd  PATH       Set the path for the data directory" << std::endl;
  std::cout << std::endl;
  std::cout << "  convert options:" << std::endl;
  std::cout << "    -i   FILE       Input file (default=input.pdb)" << std::endl;
  std::cout << "    -o   FILE       Output file (default=output.pdb)" << std::endl;
  std::cout << "    -h              Add hydrogen atoms" << std::endl;
  std::cout << "    -dd  PATH       Set the path for the data directory" << std::endl;
  std::cout << std::endl;
  std::cout << "  hydrogen options:" << std::endl;
  std::cout << "    -i   FILE       Input file (default=input.pdb)" << std::endl;
  std::cout << "    -o   FILE       Output file (default=output.pdb)" << std::endl;
  std::cout << "    -ph  VALUE      Use a specific PH value for hydrogens (default=7.0)" << std::endl;
  std::cout << "    -add            Add hydrogen atoms" << std::endl;
  std::cout << "    -del            Delete hydrogen atoms" << std::endl;
  std::cout << "    -pl             Only add polar hydrogens atoms" << std::endl;
  std::cout << "    -dd  PATH       Set the path for the data directory" << std::endl;
  std::cout << std::endl;
}

void start()
{
  // Load dummy format to force plugins load
  OpenBabel::OBConversion conv;
  OpenBabel::OBFormat *format_out = conv.FindFormat("pdb");
}

void runForcefields(int argc, char **argv)
{
  std::cout << std::endl;
  std::cout << " - Supported forcefields FFIDs:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("forcefields");
}

void runFormats(int argc, char **argv)
{
  std::cout << std::endl;
  std::cout << " - Supported file formats:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("formats");
}

void runDescriptors(int argc, char **argv)
{
  std::cout << std::endl;
  std::cout << " - Available descriptors:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("descriptors") << std::endl;
}

void runPlugins(int argc, char **argv)
{
  std::cout << std::endl;
  std::cout << " - Available loaders:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("loaders") << std::endl;
  std::cout << " - Available charges:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("charges") << std::endl;
  std::cout << " - Available ops:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("ops") << std::endl;
}

void runConfig(int argc, char **argv)
{
  // Build values
  std::cout << std::endl;
  std::cout << " - Build BABEL_DATADIR:" << std::endl;
  std::cout << BABEL_DATADIR << std::endl;
  std::cout << std::endl;
  std::cout << " - Build BABEL_VERSION:" << std::endl;
  std::cout << BABEL_VERSION << std::endl;
  std::cout << std::endl;
  std::cout << " - Build MODULE_EXTENSION:" << std::endl;
  std::cout << MODULE_EXTENSION << std::endl;
  std::cout << std::endl;
  // Env values
  std::cout << " - Environment BABEL_DATADIR:" << std::endl;
  char *env_data = getenv("BABEL_DATADIR");
  if (env_data != NULL)
  {
    std::cout << env_data << std::endl;
  }
  else
  {
    std::cout << "NOT SET" << std::endl;
  }
}

int main(int argc, char **argv)
{
  // Init lib
  start();
  // Check usage
  if (argc < 2)
  {
    usage();
    return EXIT_FAILURE;
  }
  // Check action type
  std::string action = toLower(argv[1]);
  // Minimization action
  if (action == "minimize")
  {
    runMinimize(argc, argv);
    return EXIT_SUCCESS;
  }
  // Bonding action
  if (action == "bonding")
  {
    runBonding(argc, argv);
    return EXIT_SUCCESS;
  }
  // Convert action
  if (action == "convert")
  {
    runConvert(argc, argv);
    return EXIT_SUCCESS;
  }
  // Hydrogen action
  if (action == "hydrogen")
  {
    runHydrogen(argc, argv);
    return EXIT_SUCCESS;
  }
  // Forcefield action
  if (action == "forcefields")
  {
    runForcefields(argc, argv);
    return EXIT_SUCCESS;
  }
  // Formats action
  if (action == "formats")
  {
    runFormats(argc, argv);
    return EXIT_SUCCESS;
  }
  // Descriptors action
  if (action == "descriptors")
  {
    runDescriptors(argc, argv);
    return EXIT_SUCCESS;
  }
  // Config action
  if (action == "config")
  {
    runConfig(argc, argv);
    return EXIT_SUCCESS;
  }
  // Plugin action
  if (action == "plugins")
  {
    runPlugins(argc, argv);
    return EXIT_SUCCESS;
  }
  // Action not found
  usage();
  error("Unknown action: " + action);
  return EXIT_FAILURE;
}
