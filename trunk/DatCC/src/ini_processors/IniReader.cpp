#include "IniReader.h"
#include <sstream>

namespace datcc {
 
int IniReader::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;
  return 0; //No need to do anything else here
}

int IniReader::loadFrom(const std::string &fileName) {
  return ini.LoadFile(fileName.c_str());
}

//-------- Member function template specializations --------//

template <>
int IniReader::process(Point16 &p, const std::string &key) {
  std::istringstream is(ini.GetValue(currentSection.c_str(), key.c_str(), ""));
  is >> p.x >> p.y;
  if (is.fail())
    return 1;
  else
    return 0;
}

template <>
int IniReader::process(Box16 &b, const std::string &key) {
  std::istringstream is(ini.GetValue(currentSection.c_str(), key.c_str(), ""));
  is >> b.left >> b.top >> b.right >> b.bottom;
  if (is.fail())
    return 1;
  else
    return 0;
}

} //datcc
