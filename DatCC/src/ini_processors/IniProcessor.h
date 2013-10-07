#pragma once
#include "../types.h"
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

    // Callbacks must be of the form:
    //    std::string func(int val, size_t keyStrSize);
    //template <class T, typename CallbackT>
    //int process(T &t, const std::string &key, CallbackT &commenter);
    
    //template <class T>
    //int processFlags(T &t, const std::string &key);

  protected:
    std::string currentSection;
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
