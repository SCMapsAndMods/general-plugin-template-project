#pragma once
#include <SCBW/structures/CUnit.h>

namespace hooks {

void updateStatusEffectsHook(CUnit *unit);

void injectUpdateStatusEffects();

//Wrapper for native version of updateStatusEffects()
void updateStatusEffects(CUnit *unit);

} //hooks
