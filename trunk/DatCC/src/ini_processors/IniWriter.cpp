#include "IniWriter.h"
#include <sstream>

namespace datcc {
 
int IniWriter::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;
  return ini.SetValue(currentSection.c_str(), NULL, NULL, ("; " + comment).c_str());
}

int IniWriter::saveTo(const std::string &fileName) const {
  return ini.SaveFile(fileName.c_str());
}

//-------- Member function template specializations --------//

template <>
int IniWriter::process(const std::string &str, const std::string &key) {
  return ini.SetValue(currentSection.c_str(), key.c_str(), str.c_str());
}

template <>
int IniWriter::process(const Point16 &p, const std::string &key) {
  std::ostringstream os;
  os << p.x << " " << p.y;
  return process(os.str(), key);
}

template <>
int IniWriter::process(const Box16 &b, const std::string &key) {
  std::ostringstream os;
  os << b.left << " " << b.top << " " << b.right << " " << b.bottom;
  return process(os.str(), key);
}

} //datcc
