#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void rechargeShieldsProcHook(CUnit *target, CUnit *battery);
bool unitCanRechargeShieldsHook(const CUnit *target, const CUnit *battery);
bool canStopRechargeShieldsHook(const CUnit *target, const CUnit *battery);

void injectRechargeShieldsHooks();

} //hooks
