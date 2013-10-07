#pragma once
#include "IniWriter.h"

namespace datcc {

class IniComparator: public IniWriter {
  public:
    template <class DatT>
    void compare(DatT &dat, DatT &baseDat);

    template <class T>
    int process(const T &t, const std::string &key);

    // Callbacks must be of the form:
    //    std::string func(int val, size_t keyStrSize);
    template <class T, typename CallbackT>
    int process(const T &t, const std::string &key, CallbackT &commenter);

    template <class T>
    int processFlags(const T &t, const std::string &key);

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

template <class T, typename CallbackT>
int IniComparator::process(const T &t, const std::string &key, CallbackT &commenter) {
  if (isLoadingBaseDat)
    return baseIni.SetLongValue(currentSection.c_str(), key.c_str(), t);

  const T baseVal = (T) baseIni.GetLongValue(currentSection.c_str(), key.c_str(), t);
  if (baseVal != t) {
    writeSection();
    return IniWriter::process(t, key, commenter);
  }
  else
    return 0; //Identical, no write
}

template <class T>
int IniComparator::processFlags(const T &t, const std::string &key) {
  if (isLoadingBaseDat)
    return baseIni.SetLongValue(currentSection.c_str(), key.c_str(), t);

  const T baseVal = (T) baseIni.GetLongValue(currentSection.c_str(), key.c_str(), t);
  if (baseVal != t) {
    writeSection();
    return IniWriter::processFlags(t, key);
  }
  else
    return 0; //Identical, no write
}

} //datcc
