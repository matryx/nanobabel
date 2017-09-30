
#ifndef _NANOBABEL_H
#define _NANOBABEL_H

#ifdef WIN32
  #define USING_OBDLL
#endif

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
template<typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim);

// Debug.cpp
void debugResidues(OpenBabel::OBMol *mol);
void debugAtoms(OpenBabel::OBMol *mol);
void debugBonds(OpenBabel::OBMol *mol);

// Minimize.cpp
void runMinimization(int argc, char **argv);

#endif /* _NANOBABEL_H */

