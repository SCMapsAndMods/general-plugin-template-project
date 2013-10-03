#pragma once
#include "../types.h"

class TblFile {
  public:
    int loadFile(const char *fileName);
    const char* getString(int index) const;
    const char* getEscapedString(int index) const;
    int getStringSize(int index) const;
    
    ~TblFile();

  private:
    BYTE *data;
    WORD stringCount;
    WORD *stringOffsets;
    int dataSize;
};

extern TblFile statTxtTbl;