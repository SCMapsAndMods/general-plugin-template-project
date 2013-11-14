#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

bool unitCanAttackInsideBunkerHook(const CUnit *unit);
void createBunkerAttackThingyHook(const CUnit *unit);

void injectBunkerHooks();

} //hooks
