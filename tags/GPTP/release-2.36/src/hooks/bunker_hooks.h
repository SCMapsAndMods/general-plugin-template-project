#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

bool unitCanAttackInsideBunkerHook(const CUnit *unit);
void applyBunkerAttackAnimationHook(CUnit *unit);

void injectBunkerHooks();

} //hooks
