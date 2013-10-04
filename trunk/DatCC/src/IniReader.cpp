#include "IniReader.h"

namespace datcc {
 
int IniReader::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;
  return 0; //No need to do anything else here
}

int IniReader::loadFrom(const std::string &fileName) {
  return ini.LoadFile(fileName.c_str());
}

//-------- Member function template specialization --------//

template <>
int IniReader::process(std::string &str, const std::string &key) {
  str = ini.GetValue(currentSection.c_str(), key.c_str(), str.c_str());
  return 0;
}

} //datcc
