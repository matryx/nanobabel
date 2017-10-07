
#ifndef _NANOBABEL_H
#define _NANOBABEL_H

#ifdef WIN32
  #define USING_OBDLL
#endif

#include <cstdlib> // getenv
#include <algorithm>
#include <string>

#include <openbabel/babelconfig.h>
#include <openbabel/base.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/oberror.h>

#ifndef _MSC_VER
  #include <unistd.h>
#endif

// Misc.cpp
void usage();
void error(std::string line);
void log(std::string line);
std::string readFile(std::string filename);
void writeFile(std::string filename, std::string content);
std::string toString(int i);
std::string toString(double i);
std::string toString(char i);
std::string toLower(std::string s);
std::vector<std::string> split(const std::string &s, char delim);
int setenv(const char *name, const char *value, int overwrite);

// Debug.cpp
void debugResidues(OpenBabel::OBMol *mol);
void debugAtoms(OpenBabel::OBMol *mol);
void debugBonds(OpenBabel::OBMol *mol);

// Minimize.cpp
void runMinimization(int argc, char **argv);

// Convert.cpp
void runConvert(int argc, char **argv);

// Bonding.cpp
void runBonding(int argc, char **argv);

// Constraints.cpp
bool applyConstraints(OpenBabel::OBMol *mol, OpenBabel::OBFFConstraints *constraints, std::string constraintFileContent);

#endif /* _NANOBABEL_H */

