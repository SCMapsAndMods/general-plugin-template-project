#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

bool unitIsConsumable(const CUnit* unit, u8 consumingPlayer);
void onConsumeUnit(CUnit* caster, CUnit *target);

void injectConsumeHooks();

} //hooks
