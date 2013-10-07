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

  private:
    CSimpleIniCaseA baseIni;
    bool isLoadingBaseDat;
};

//-------- Member function template definitions --------//

template <class DatT>
void IniComparator::compare(DatT &dat, DatT &baseDat) {
  isLoadingBaseDat = true;
  dat.processIni(*this);

  isLoadingBaseDat = false;
  dat.processIni(*this);
}

template <class T>
int IniComparator::process(const T &t, const std::string &key) {
  if (isLoadingBaseDat)
    return baseIni.SetLongValue(currentSection.c_str(), key.c_str(), t);
  
  const T baseVal = (T) baseIni.GetLongValue(currentSection.c_str(), key.c_str(), t);
  if (baseVal != t)
    return ini.SetLongValue(currentSection.c_str(), key.c_str(), t);
  else
    return 0; //Identical, no write
}

template <class T, typename CallbackT>
int IniComparator::process(const T &t, const std::string &key, CallbackT &commenter) {
  return process(t, key);
}

template <class T>
int IniComparator::processFlags(const T &t, const std::string &key) {
  return process(t, key);
}

} //datcc
