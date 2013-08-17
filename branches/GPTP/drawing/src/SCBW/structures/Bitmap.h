//Based on BWAPI's BW/Bitmap.h
#pragma once
#include "../../types.h"
#pragma pack(1)

class Bitmap {
  public:
    bool blitString(const char *pszStr, int x, int y, u8 size);
    u16 getWidth() const;
    u16 getHeight() const;

  private:
    u16 width, height;
    u8* data;
};

#pragma pack()
