//Common functions used by the AI.
#pragma once
//#include "../SCBW/UnitFinder.h"
#include "../SCBW/structures/CUnit.h"

namespace AI {

bool isTargetWorthHitting(const CUnit *unit, const CUnit *target);

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster);

} //AI
