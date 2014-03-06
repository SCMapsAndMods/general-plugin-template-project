#pragma once
#include <SCBW/structures/CUnit.h>

namespace hooks {

u8 getArmorBonusHook(const CUnit *unit);

void injectArmorBonusHook();

} //hooks
