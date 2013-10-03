#include "IniReader.h"

namespace datcc {
 
int IniReader::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;
  return ini.SetValue(currentSection.c_str(), NULL, NULL, comment.c_str());
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
