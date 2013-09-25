#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void regenerateEnergyHook(CUnit *unit);

void injectEnergyRegenerationHook();

//Wrapper for native version of regenerateEnergy()
void regenerateEnergy(CUnit *unit);

} //hooks
