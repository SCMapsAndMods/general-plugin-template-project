#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void damageWithHook(s32     damage,
                    CUnit*  target,
                    u8      weaponId,
                    CUnit*  attacker,
                    s8      attackingPlayer,
                    s8      direction,
                    u8      dmgDivisor
                    );

void injectDamageWithHook();

} //hooks
