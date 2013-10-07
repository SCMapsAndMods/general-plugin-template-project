#pragma once
#include "IniProcessor.h"
#include "ini_comments.h"

namespace datcc {

class IniWriter: public IniProcessor {
  public:
    int setSection(const std::string &section, const std::string &comment);

    template <class T>
    int process(const T &t, const std::string &key);

    template <class T, typename CallbackT>
    int process(const T &t, const std::string &key, CallbackT &commenter);

    template <class T>
    int processFlags(const T &t, const std::string &key);

    int saveTo(const std::string &fileName) const;
};

//-------- Member function template definitions --------//

template <class T>
int IniWriter::process(const T &t, const std::string &key) {
  return ini.SetLongValue(currentSection.c_str(), key.c_str(), t);
}

template <>
int IniWriter::process(const std::string &str, const std::string &key);

template <class T, typename CallbackT>
int IniWriter::process(const T &t, const std::string &key, CallbackT &commenter) {
  return process(commenter(t, key.size()), key);
}

template <class T>
int IniWriter::processFlags(const T &t, const std::string &key) {
  return process(std::string(convertFlagsToStr(t)), key);
}

} //datcc
