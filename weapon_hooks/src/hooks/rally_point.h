#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void orderNewUnitToRally(CUnit* unit, CUnit* factory);
void setRallyPosition(CUnit* unit, u16 x, u16 y);
void setRallyUnit(CUnit* unit, CUnit* target);

void injectRallyHooks();

} //hooks
