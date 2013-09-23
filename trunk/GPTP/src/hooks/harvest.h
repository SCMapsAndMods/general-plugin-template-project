#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

u8 harvestResourceHook(CUnit *resource, bool isMineral);

void harvestResourceInject();

} //hooks
