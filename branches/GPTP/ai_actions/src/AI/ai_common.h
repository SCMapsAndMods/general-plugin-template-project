//Common functions used by the AI.
#pragma once
#include "../SCBW/UnitFinder.h"
#include "../SCBW/structures/CUnit.h"
#include "../SCBW/api.h"

namespace AI {

bool isTargetWorthHitting(const CUnit *target, const CUnit *attacker);

const CUnit* findBestSpellTarget(int x, int y, int searchBounds,
  scbw::UnitFinderCallbackMatchInterface &callback);

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster);

//-------- Template function definition --------//

} //AI
