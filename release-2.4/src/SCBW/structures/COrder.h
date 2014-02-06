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

static_assert(sizeof(COrder) == 20, "The size of the COrder structure is invalid");

#pragma pack()
