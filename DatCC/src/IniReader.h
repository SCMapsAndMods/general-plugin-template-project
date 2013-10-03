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

    template <class T>
    int processTblId(T &t, const std::string &key);

    int loadFrom(const std::string &fileName);

  private:
    std::string currentSection;
};


//-------- Member function template definitions --------//

template <class T>
int IniReader::process(T &t, const std::string &key) {
  t = (T) ini.GetLongValue(currentSection.c_str(), key.c_str(), t);
  return 0;
}

template <>
int IniReader::process(std::string &str, const std::string &key);

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

template <class T>
int IniReader::processTblId(T &t, const std::string &key) {
  return process(t, key);
}

} //datcc
