#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

u8 getSpiderMineBurrowTimeHook(const CUnit *spiderMine);
bool spiderMineCanTargetUnitHook(const CUnit *target, const CUnit *spiderMine);

void injectSpiderMineHooks();

} //hooks
