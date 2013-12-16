#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void doIrradiateDamageHook(CUnit *unit);

void injectIrradiateHook();

//Wrapper for native version of doIrradiateDamage()
void doIrradiateDamage(CUnit *unit);

} //hooks
