#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

CUnit* findBestSpiderMineTargetHook(const CUnit *mine);
u8 getSpiderMineBurrowTimeHook(const CUnit *spiderMine);

void injectSpiderMineHooks();

} //hooks
