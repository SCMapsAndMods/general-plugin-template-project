#include "IniComparator.h"
#include <sstream>

namespace datcc {

int IniComparator::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;

  if (!isLoadingBaseDat) {
    currentSectionComment = comment;
    isCurrentSectionUnwritten = true;
  }

  return 0;
}

void IniComparator::writeSection() {
  if (isCurrentSectionUnwritten) {
    isCurrentSectionUnwritten = false;
    IniWriter::setSection(currentSection, currentSectionComment);
  }
}

//-------- Member function template specializations --------//

template <>
int IniComparator::process(const Point16 &p, const std::string &key) {
  std::ostringstream os;
  os << p.x << " " << p.y;

  if (isLoadingBaseDat)
    return baseIni.SetValue(currentSection.c_str(), key.c_str(), os.str().c_str());

  const std::string val = os.str();
  const std::string baseVal(baseIni.GetValue(currentSection.c_str(), key.c_str(), ""));
  if (baseVal != val) {
    writeSection();
    return IniWriter::process(val, key);
  }
  else
    return 0; //Identical, no write
}
} //datcc
