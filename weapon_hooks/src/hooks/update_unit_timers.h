#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void updateUnitTimersHook(CUnit* unit);

void injectUpdateUnitTimers();

} //hooks
