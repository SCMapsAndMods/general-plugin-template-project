#pragma once
#include "IniProcessor.h"
#include "parser.h"

namespace datcc {

class IniReader: public IniProcessor {
  public:
    int setSection(const std::string &section, const std::string &comment);

    template <class T>
    int process(T &t, const std::string &key);

    template <class T, typename CallbackT>
    int process(T &t, const std::string &key, CallbackT &commenter);

    template <class T>
    int processFlags(T &t, const std::string &key);

    int loadFrom(const std::string &fileName);
};


//-------- Member function template definitions --------//

template <class T>
int IniReader::process(T &t, const std::string &key) {
  const char *str = ini.GetValue(currentSection.c_str(), key.c_str());
  if (str != NULL)
    t = (T) atol(str);  //To handle inline comments
  return 0;
}

template <>
int IniReader::process(Point16 &p, const std::string &key);

template <class T, typename CallbackT>
int IniReader::process(T &t, const std::string &key, CallbackT &commenter) {
  return process(t, key);
}

template <class T>
int IniReader::processFlags(T &t, const std::string &key) {
  const char* flagStr = ini.GetValue(currentSection.c_str(), key.c_str());
  if (flagStr != NULL)
    t = (T) datcc::convertToFlags(flagStr);
  return 0;
}

} //datcc
