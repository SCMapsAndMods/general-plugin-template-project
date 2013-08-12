#pragma once
#include "Target.h"

#pragma pack(1)

struct COrder {
  COrder*   prev;
  COrder*   next;
  u16       orderId;
  u16       unitId;
  Target    target;
};

C_ASSERT(sizeof(COrder) == 20);

#pragma pack()
