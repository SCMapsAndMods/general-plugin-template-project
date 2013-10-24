//Common functions used by the AI.
#pragma once
#include "../SCBW/UnitFinder.h"
#include "../SCBW/structures/CUnit.h"
#include "../SCBW/api.h"

namespace AI {

bool isTargetWorthHitting(const CUnit *target, const CUnit *attacker);

//Determines whether the given @p playerId is in a UMS map (i.e. campaign mode).
bool isUmsMode(s8 playerId);

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster);

//-------- Template function definition --------//

} //AI
