#pragma once
#include "types.h"
#include <SimpleIni.h>
#include <string>

namespace datcc {

class IniProcessor {
  public:
    /// Sets the current section of the INI file and optionally adds a comment.
    /// @return Error code (0 for no error)
    virtual int setSection(const std::string &section, const std::string &comment) = 0;
    
    //template <class T>
    //int process(T &t, const std::string &key);

    //template <class T>
    //int processUnitId(T &t, const std::string &key);
    
    //template <class T>
    //int processWeaponId(T &t, const std::string &key);
    
    //template <class T>
    //int processFlingyId(T &t, const std::string &key);

    //template <class T>
    //int processSpriteId(T &t, const std::string &key);
    
    //template <class T>
    //int processImageId(T &t, const std::string &key);
    
    //template <class T>
    //int processUpgradeId(T &t, const std::string &key);

    //template <class T>
    //int processOrderId(T &t, const std::string &key);
    
    //template <class T>
    //int processFlags(T &t, const std::string &key);

    //template <class T>
    //int processTblId(T &t, const std::string &key);

  private:
    std::string currentSection;

  protected:
    CSimpleIniCaseA ini;
};

/*
class IniImporter {
  public:
    void setSection(const std::string &section, const std::string &comment);

  private:
    std::string currentSection;
};

class DifferentialExporter {
  public:
    void setSection(const std::string &section, const std::string &comment);

  private:
    std::string currentSection;
};
*/
} //datcc
