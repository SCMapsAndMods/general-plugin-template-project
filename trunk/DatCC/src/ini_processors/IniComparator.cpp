#include "IniComparator.h"

namespace datcc {

int IniComparator::setSection(const std::string &section, const std::string &comment) {
  currentSection = section;

  if (!isLoadingBaseDat) {
    currentSectionComment = comment;
    isCurrentSectionUnwritten = true;
  }

  return 0;
}

} //datcc
