#include "nanobabel.h"

void error(std::string line)
{
  std::cerr << " ERROR: nanobabel: " << line << std::endl;
  exit(-1);
}

void log(std::string line)
{
  std::cout << " nanobabel: " << line << std::endl;
}

std::string readFile(std::string filename)
{
  std::ifstream ifs;
  ifs.open(filename.c_str());
  if (!ifs)
  {
    error("cannot read file: " + filename);
  }
  std::stringstream stream;
  stream << ifs.rdbuf();
  std::string content = stream.str();
  ifs.close();
  return content;
}

void writeFile(std::string filename, std::string content)
{
  std::ofstream ofs;
  ofs.open(filename.c_str());
  if (!ofs)
  {
    error("cannot write file: " + filename);
  }
  ofs << content;
  ofs.close();
}

std::string toString(char *i)
{
  if (i == NULL)
  {
    return "NULL";
  }
  std::string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}

std::string toString(int i)
{
  std::string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}

std::string toString(double i)
{
  std::string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}

std::string toString(char i)
{
  std::string s;
  std::stringstream out;
  out << i;
  s = out.str();
  return s;
}

std::string toLower(std::string s)
{
  std::string tmp = s;
  std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
  return tmp;
}

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

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int setenv(const char *name, const char *value, int overwrite) throw()
{
    char* envVar = 0;
    if(!overwrite)
    {
        size_t envsize = 0;
        envVar = getenv(name);
        if (envVar == (char*)0)
        {
          return -1;
        }
    }
    return setenv(name, value, 1);
}
