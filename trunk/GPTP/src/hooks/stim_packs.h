#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void useStimPacksHook(CUnit *unit);
bool canUseStimPacksHook(const CUnit *unit);

void injectStimPacksHooks();

} //hooks
