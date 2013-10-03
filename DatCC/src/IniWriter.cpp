#include "IniWriter.h"

namespace datcc {
 
int IniWriter::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;
  return ini.SetValue(currentSection.c_str(), NULL, NULL, comment.c_str());
}

int IniWriter::saveTo(const std::string &fileName) const {
  return ini.SaveFile(fileName.c_str());
}

//-------- Member function template specialization --------//

template <>
int IniWriter::process(const std::string &str, const std::string &key) {
  return ini.SetValue(currentSection.c_str(), key.c_str(), str.c_str());
}

} //datcc
