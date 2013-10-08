#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

u8 getCloakingTech(const CUnit *unit);

void injectCloakingTechHooks();

} //hooks
