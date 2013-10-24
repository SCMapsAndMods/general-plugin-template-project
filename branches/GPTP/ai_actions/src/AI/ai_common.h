//Common functions used by the AI.
#pragma once
#include "../SCBW/UnitFinder.h"
#include "../SCBW/structures/CUnit.h"
#include "../SCBW/api.h"

namespace AI {

bool isTargetWorthHitting(const CUnit *target, const CUnit *attacker);

/// Checks whether the unit has a means of attacking.
/// Carriers and Reavers are checked for available Scarabs and Interceptors.
/// Lurkers are checked whether they are burrowed.
bool unitCanAttack(const CUnit *unit);

/// Determines whether the given @p playerId is in a UMS map (i.e. campaign mode).
bool isUmsMode(s8 playerId);

/// Returns the total HP shown in-game.
int getCurrentHpInGame(const CUnit *unit);

/// Returns the total HP + Shields of this unit (HP only if unit has no shield).
/// This returns in-game values, NOT internal values.
int getCurrentLifeInGame(const CUnit *unit);

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster);

//-------- Template function definition --------//

} //AI
