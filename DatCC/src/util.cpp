#include "util.h"
#include <algorithm>


bool compareIgnoreCase(char a, char b) {
  return tolower(a) == tolower(b);
}

const char INI_EXT[] = ".ini";
const char DAT_EXT[] = ".dat";

std::string getOutputIniPath(const std::string &inputDatPath) {
  //Strip .DAT extension
  std::string::const_iterator i = std::find_end(
                inputDatPath.begin(), inputDatPath.end(),
                &DAT_EXT[0], &DAT_EXT[sizeof(DAT_EXT) - 1], compareIgnoreCase);

  //Append .INI extension
  return inputDatPath.substr(0, i - inputDatPath.begin()).append(INI_EXT);
}

std::string getOutputDatPath(const std::string &inputIniPath) {
  //Strip .INI extension
  std::string::const_iterator i = std::find_end(
                inputIniPath.begin(), inputIniPath.end(),
                &INI_EXT[0], &INI_EXT[sizeof(INI_EXT) - 1], compareIgnoreCase);

  //Append DAT extension
  return inputIniPath.substr(0, i - inputIniPath.begin()).append(DAT_EXT);
}


unsigned int getFileSize(std::ifstream &file) {
  if (file.bad())
    return 0;
  const int prevPos = file.tellg();
  file.seekg(0, file.end);
  unsigned int size = file.tellg();
  file.seekg(prevPos);
  return size;
}
