
#include "nanobabel.h"

void usage()
{
  std::cout << "Usage: nanobabel [action] [options]" << std::endl;
  std::cout << std::endl;
  std::cout << "  action:" << std::endl;
  std::cout << "    MINIMIZE      Molecular energy minimization" << std::endl;
  std::cout << "    CONVERT       Rewrite molecule file in a different format" << std::endl;
  std::cout << "    FORCEFIELDS   List supported forcefields" << std::endl;
  std::cout << "    FORMATS       List supported file formats" << std::endl;
  std::cout << "    PLUGINS       List supported plugins" << std::endl;
  std::cout << "    CONFIG        Current configuration informations" << std::endl;
  std::cout << std::endl;
  std::cout << "  MINIMIZE options:" << std::endl;
  std::cout << "    -i  FILE      Input file (default=input.pdb)" << std::endl;
  std::cout << "    -o  FILE      Output file (default=output.pdb)" << std::endl;
  std::cout << "    -cx FILE      Forcefield constraints file" << std::endl;
  std::cout << "    -ff FFID      Select a forcefield (default=UFF)" << std::endl;
  std::cout << "    -h            Add hydrogen atoms" << std::endl;
  std::cout << "    -n  steps     Specify the maximum number of steps (default=2500)" << std::endl;
  std::cout << "    -l  steps     Specify the interval number of steps between structure updates" << std::endl;
  std::cout << "    -dd PATH      Set the path for the data directory" << std::endl;
  std::cout << std::endl;
  std::cout << "  CONVERT options:" << std::endl;
  std::cout << "    -i  FILE      Input file (default=input.pdb)" << std::endl;
  std::cout << "    -o  FILE      Output file (default=output.pdb)" << std::endl;
  std::cout << "    -h            Add hydrogen atoms" << std::endl;
  std::cout << "    -dd PATH      Set the path for the data directory" << std::endl;
}

void start()
{
  // Load dummy format to force plugin load
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

void runConfig(int argc, char **argv)
{
  std::cout << std::endl;
  std::cout << " - Environment BABEL_DATADIR:" << std::endl;
  std::cout << getenv("BABEL_DATADIR") << std::endl;
  std::cout << std::endl;
  std::cout << " - Build BABEL_DATADIR:" << std::endl;
  std::cout << BABEL_DATADIR << std::endl;
  std::cout << std::endl;
  std::cout << " - Build BABEL_VERSION:" << std::endl;
  std::cout << BABEL_VERSION << std::endl;
  std::cout << std::endl;
  std::cout << " - Build MODULE_EXTENSION:" << std::endl;
  std::cout << MODULE_EXTENSION << std::endl;
}

void runPlugins(int argc, char **argv)
{
  std::cout << std::endl;
  std::cout << " - Available loaders:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("loaders") << std::endl;
  std::cout << " - Available charges:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("charges") << std::endl;
  std::cout << " - Available descriptors:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("descriptors") << std::endl;
  std::cout << " - Available ops:" << std::endl;
  std::cout << OpenBabel::OBPlugin::ListAsString("ops") << std::endl;
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
  std::string action = argv[1];
  // Minimization action
  if (action == "MINIMIZE")
  {
    runMinimization(argc, argv);
    return EXIT_SUCCESS;
  }
  // Minimization action
  if (action == "CONVERT")
  {
    runConvert(argc, argv);
    return EXIT_SUCCESS;
  }
  // Forcefield action
  if (action == "FORCEFIELDS")
  {
    runForcefields(argc, argv);
    return EXIT_SUCCESS;
  }
  // Forcefield action
  if (action == "FORMATS")
  {
    runFormats(argc, argv);
    return EXIT_SUCCESS;
  }
  // Forcefield action
  if (action == "CONFIG")
  {
    runConfig(argc, argv);
    return EXIT_SUCCESS;
  }
  // Forcefield action
  if (action == "PLUGINS")
  {
    runPlugins(argc, argv);
    return EXIT_SUCCESS;
  }
  // Action not found
  error("Unknown action: " + action);
  return EXIT_FAILURE;
}
