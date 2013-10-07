#pragma once
#include "IniWriter.h"

namespace datcc {

class IniComparator: public IniWriter {
  public:
    template <class DatT>
    void compare(DatT &dat, DatT &baseDat);

    //template <class T>
    //int process(T &t, const std::string &key);

    // Callbacks must be of the form:
    //    std::string func(int val, size_t keyStrSize);
    //template <class T, typename CallbackT>
    //int process(T &t, const std::string &key, CallbackT &commenter);
    
    //template <class T>
    //int processFlags(T &t, const std::string &key);
  private:
    std::string currentSection;
};

} //datcc
