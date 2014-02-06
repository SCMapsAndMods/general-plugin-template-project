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

/// Returns true if @p target is attacking a player allied to @p unit.
bool isTargetAttackingAlly(const CUnit *target, const CUnit *unit);

/// Determines whether the given @p playerId is in a UMS map (i.e. campaign mode).
bool isUmsMode(u8 playerId);

/// Determines if the @p unit is within a region that is not commanded by its
/// controlling AI. Details are not really understood.
bool isUnitInUnsafeRegion(const CUnit *unit);

/// Returns the total HP shown in-game.
int getCurrentHpInGame(const CUnit *unit);

/// Returns the maximum HP shown in-game.
int getMaxHpInGame(const CUnit *unit);

/// Returns the total HP + Shields of this unit (HP only if unit has no shield).
/// This returns in-game values, NOT internal values.
int getCurrentLifeInGame(const CUnit *unit);

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId);
int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyNukeValueInArea(int x, int y, int searchBounds, const CUnit *caster);

//-------- Template function definition --------//

} //AI
