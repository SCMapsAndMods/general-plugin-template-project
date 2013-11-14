#pragma once
#include "IniProcessor.h"
#include "../flags.h"

namespace datcc {

class IniWriter: public IniProcessor {
  public:
    int setSection(const std::string &section, const std::string &comment);

    template <class T>
    int process(const T &t, const std::string &key);

    template <class T>
    int process(const T &t, const std::string &key, CommentFunc commenter);

    template <class T>
    int process(const T &t, const std::string &key, const FlagNames<T> &flagNames);

    int saveTo(const std::string &fileName) const;
};

//-------- Member function template definitions --------//

template <class T>
int IniWriter::process(const T &t, const std::string &key) {
  return ini.SetLongValue(currentSection.c_str(), key.c_str(), t);
}

template <>
int IniWriter::process(const std::string &str, const std::string &key);

template <>
int IniWriter::process(const Point16 &p, const std::string &key);

template <>
int IniWriter::process(const Box16 &b, const std::string &key);

template <class T>
int IniWriter::process(const T &t, const std::string &key, CommentFunc commenter) {
  return process(commenter(t, key.size()), key);
}

template <class T>
int IniWriter::process(const T &t, const std::string &key, const FlagNames<T> &flagNames) {
  return process(flagNames.makeCommentString(t), key);
}

} //datcc
