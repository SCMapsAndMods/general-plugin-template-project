#pragma once
#include "../types.h"

namespace datcc {

/// Wrapper for *.TBL files. WARNING: TBL file indices start at 1, NOT 0!

class TblFile {
  public:
    int loadFile(const char *fileName);
    const char* getString(int index) const;
    const char* getEscapedString(int index) const;
    size_t getStringSize(int index) const;
    
    ~TblFile();

  private:
    BYTE *data;
    WORD stringCount;
    WORD *stringOffsets;
    int dataSize;
};

extern TblFile statTxtTbl, imagesTbl;

} //datcc
