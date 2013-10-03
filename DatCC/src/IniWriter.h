#pragma once
#include "IniProcessor.h"
#include "ini_comments.h"

namespace datcc {

class IniWriter: public IniProcessor {
  public:
    int setSection(const std::string &section, const std::string &comment);

    template <class T>
    int process(const T &t, const std::string &key);

    template <class T>
    int processUnitId(const T &t, const std::string &key);
    
    template <class T>
    int processWeaponId(const T &t, const std::string &key);
    
    template <class T>
    int processFlingyId(const T &t, const std::string &key);
    
    template <class T>
    int processSpriteId(const T &t, const std::string &key);
    
    template <class T>
    int processImageId(const T &t, const std::string &key);
    
    template <class T>
    int processUpgradeId(const T &t, const std::string &key);

    template <class T>
    int processOrderId(const T &t, const std::string &key);
    
    template <class T>
    int processFlags(const T &t, const std::string &key);

    template <class T>
    int processTblId(const T &t, const std::string &key);

    int IniWriter::saveTo(const std::string &fileName) const;

  private:
    std::string currentSection;
};

//-------- Member function template definitions --------//

template <class T>
int IniWriter::process(const T &t, const std::string &key) {
  return ini.SetLongValue(currentSection.c_str(), key.c_str(), t);
}

template <>
int IniWriter::process(const std::string &str, const std::string &key);

template <class T>
int IniWriter::processUnitId(const T &t, const std::string &key) {
  return process(makeUnitComment(t, key.size()), key);
}

template <class T>
int IniWriter::processWeaponId(const T &t, const std::string &key) {
  return process(makeWeaponComment(t, key.size()), key);
}

template <class T>
int IniWriter::processFlingyId(const T &t, const std::string &key) {
  return process(makeFlingyComment(t, key.size()), key);
}

template <class T>
int IniWriter::processSpriteId(const T &t, const std::string &key) {
  return process(makeSpriteComment(t, key.size()), key);
}

template <class T>
int IniWriter::processImageId(const T &t, const std::string &key) {
  return process(makeImageComment(t, key.size()), key);
}

template <class T>
int IniWriter::processUpgradeId(const T &t, const std::string &key) {
  return process(makeUpgradeComment(t, key.size()), key);
}

template <class T>
int IniWriter::processOrderId(const T &t, const std::string &key) {
  return process(makeOrderComment(t, key.size()), key);
}

template <class T>
int IniWriter::processFlags(const T &t, const std::string &key) {
  return process(std::string(convertFlagsToStr(t)), key);
}

template <class T>
int IniWriter::processTblId(const T &t, const std::string &key) {
  return process(makeTblComment(t, key.size()), key);
}

} //datcc
