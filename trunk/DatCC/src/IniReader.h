#include "IniProcessor.h"
#include "parser.h"

namespace datcc {

class IniReader: public IniProcessor {
  public:
    int setSection(const std::string &section, const std::string &comment);

    template <class T>
    int process(T &t, const std::string &key);

    template <class T>
    int processUnitId(T &t, const std::string &key);
    
    template <class T>
    int processWeaponId(T &t, const std::string &key);
    
    template <class T>
    int processFlingyId(T &t, const std::string &key);

    template <class T>
    int processSpriteId(T &t, const std::string &key);
    
    template <class T>
    int processImageId(T &t, const std::string &key);
    
    template <class T>
    int processUpgradeId(T &t, const std::string &key);

    template <class T>
    int processOrderId(T &t, const std::string &key);
    
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

template <class T>
int IniReader::processUnitId(T &t, const std::string &key) {
  return process(t, key);
}

template <class T>
int IniReader::processWeaponId(T &t, const std::string &key) {
  return process(t, key);
}

template <class T>
int IniReader::processFlingyId(T &t, const std::string &key) {
  return process(t, key);
}

template <class T>
int IniReader::processSpriteId(T &t, const std::string &key) {
  return process(t, key);
}

template <class T>
int IniReader::processImageId(T &t, const std::string &key) {
  return process(t, key);
}

template <class T>
int IniReader::processUpgradeId(T &t, const std::string &key) {
  return process(t, key);
}

template <class T>
int IniReader::processOrderId(T &t, const std::string &key) {
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
