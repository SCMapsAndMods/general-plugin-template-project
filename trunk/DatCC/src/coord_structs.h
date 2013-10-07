#pragma once
#include "types.h"

namespace datcc {

#pragma pack(1)

struct Point16 {
  SWORD x;
  SWORD y;
};

struct Box16 {
  SWORD left;
  SWORD top;
  SWORD right;
  SWORD bottom;
};

#pragma pack()

C_ASSERT(sizeof(Point16) == 4);
C_ASSERT(sizeof(Box16) == 8);

} //datcc
