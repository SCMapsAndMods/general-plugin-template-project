#pragma once
#include "IniWriter.h"

namespace datcc {

class IniComparator: public IniWriter {
  public:
    template <class DatT>
    void compare(DatT &dat, DatT &baseDat);

    template <class T>
    int process(const T &t, const std::string &key);

    //Process callbacks and FlagNames structures
    template <class T, class T2>
    int process(const T &t, const std::string &key, const T2 &dummy);

    int setSection(const std::string &section, const std::string &comment);

  private:
    void writeSection();

    CSimpleIniCaseA baseIni;
    std::string currentSectionComment;
    bool isLoadingBaseDat;
    bool isCurrentSectionUnwritten;
};

//-------- Member function template definitions --------//

template <class DatT>
void IniComparator::compare(DatT &dat, DatT &baseDat) {
  isLoadingBaseDat = true;
  baseDat.processIni(*this);

  isLoadingBaseDat = false;
  dat.processIni(*this);
}

template <class T>
int IniComparator::process(const T &t, const std::string &key) {
  if (isLoadingBaseDat)
    return baseIni.SetLongValue(currentSection.c_str(), key.c_str(), t);
  
  const T baseVal = (T) baseIni.GetLongValue(currentSection.c_str(), key.c_str(), t);
  if (baseVal != t) {
    writeSection();
    return IniWriter::process(t, key);
  }
  else
    return 0; //Identical, no write
}

template <>
int IniComparator::process(const Point16 &p, const std::string &key);

template <>
int IniComparator::process(const Box16 &b, const std::string &key);

template <class T, class T2>
int IniComparator::process(const T &t, const std::string &key, const T2 &dummy) {
  return process(t, key);
}

} //datcc
