#pragma once
#include "types.h"
#include "ini_comments.h"

namespace datcc {

class DatInterface {
  public:
    /// Retrieve the pointer to internal data structure.
    virtual void* getData() = 0;
    virtual const void* getData() const = 0;

    /// Retrieve the size of the internal data structure.
    virtual int getDataSize() const = 0;

    /// Use IniProcessor to process INI file.
    //template <class IniProcT>
    //void processIni(IniProcT &iniProc);
};

} //datcc
