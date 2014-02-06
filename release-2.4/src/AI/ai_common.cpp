#include "ai_common.h"
#include <SCBW/enumerations.h>
#include <SCBW/api.h>
#include <SCBW/UnitFinder.h>
#include <cassert>

namespace AI {

bool isTargetWorthHitting(const CUnit *target, const CUnit *attacker) {
  //If the target is hidden by the fog-of-war
  if (!target->sprite->isVisibleTo(attacker->playerId))
    return false;

  //If the target is not detectable
  if (target->status & (UnitStatus::Cloaked | UnitStatus::RequiresDetection)
      && !target->isVisibleTo(attacker->playerId))
    return false;

  if (scbw::isAlliedTo(attacker->playerId, target->getLastOwnerId()))
    return false;

  if (target->id == UnitId::scarab
      || target->id == UnitId::spider_mine
      || target->id == UnitId::interceptor)
    return false;

  //Target is a worker inside a gas building
  if (target->mainOrderId == OrderId::HarvestGas1
      || target->mainOrderId == OrderId::HarvestGas2
      || target->mainOrderId == OrderId::HarvestGas3)
    return false;

  return true;
}

//Logically equivalent to function @ 0x00476180
bool unitCanAttack(const CUnit *unit) {
  if (unit->id == UnitId::lurker) {
    if (unit->status & UnitStatus::Burrowed
        && units_dat::GroundWeapon[unit->id] != WeaponId::None)
      return true;
  }
  else if (unit->id == UnitId::carrier || unit->id == UnitId::gantrithor) {
    if ((unit->carrier.inHangarCount + unit->carrier.outHangarCount) > 0)
      return true;
  }
  else if (unit->id == UnitId::reaver || unit->id == UnitId::warbringer) {
    if (unit->carrier.inHangarCount > 0)
      return true;
  }
  else {
    if (units_dat::GroundWeapon[unit->id] != WeaponId::None
        || units_dat::AirWeapon[unit->id] != WeaponId::None)
      return true;
  }

  return false;
}

bool isTargetAttackingAlly(const CUnit *target, const CUnit *unit) {
  if (CUnit *secondTarget = target->orderTarget.unit) {
    if (secondTarget->playerId < 8
        && scbw::isAlliedTo(unit->playerId, secondTarget->getLastOwnerId()))
      return true;
  }

  return false;
}

bool isUmsMode(u8 playerId) {
  assert(playerId < 8);
  return AIScriptController[playerId].AI_Flags.isUseMapSettings;
}

//-------- isUnitInUnsafeRegion() --------//

//Internal use only
const u32 Func_GetRegionIdAtPosEx = 0x0049C9F0;
u16 getRegionIdAtPosEx(s32 x, s32 y) {
  static u16 result;

  __asm {
    PUSHAD
    MOV EDI, x
    MOV ECX, y
    CALL Func_GetRegionIdAtPosEx
    MOV result, AX
    POPAD
  }

  return result;
}

//Based on code @ 0x00440BB0
bool isUnitInUnsafeRegion(const CUnit *unit) {
  u16 currentRegion = getRegionIdAtPosEx(unit->getX(), unit->getY());
  AiCaptain *currentAiCaptain = &AiRegionCaptains[unit->playerId][currentRegion];
  return currentAiCaptain->captainType == 3
    || currentAiCaptain->unknown_0x1C
    || currentAiCaptain->unknown_0x20
    || currentAiCaptain->captainFlags & 0x20;
}

//-------- Unit stats shown in-game --------//

int getCurrentHpInGame(const CUnit *unit) {
  return (unit->hitPoints + 255) / 256;
}

int getMaxHpInGame(const CUnit *unit) {
  return (units_dat::MaxHitPoints[unit->id] + 255) / 256;
}

int getCurrentLifeInGame(const CUnit *unit) {
  if (units_dat::ShieldsEnabled[unit->id])
    return getCurrentHpInGame(unit) + unit->shields / 256;
  else
    return getCurrentHpInGame(unit);
}


//-------- Get total unit stats in area --------//

scbw::UnitFinder unitStatTotalFinder;

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);

  int totalEnemyLife = 0;

  unitStatTotalFinder.forEach([&caster, &weaponId, &totalEnemyLife] (const CUnit *target) {
    if (target == caster)
      return;

    if (target->status & UnitStatus::Invincible)
      return;

    if (!scbw::canWeaponTargetUnit(weaponId, target, caster))
      return;

    if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return;

    if (weaponId == WeaponId::Plague)
      totalEnemyLife += getCurrentHpInGame(target);
    else if (weaponId == WeaponId::Maelstrom) {
      if (units_dat::BaseProperty[target->id] & UnitProperty::Organic
          && target->maelstromTimer == 0) {
        totalEnemyLife += getCurrentLifeInGame(target);
      }
    }
    else
      totalEnemyLife += getCurrentLifeInGame(target);
  });

  return totalEnemyLife;
}

int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);

  int totalAllyLife = 0;

  unitStatTotalFinder.forEach([&caster, &weaponId, &totalAllyLife] (const CUnit *target) {
    if (target == caster)
      return;

    if (target->status & UnitStatus::Invincible)
      return;

    if (!scbw::canWeaponTargetUnit(weaponId, target, caster))
      return;

    if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return;

    totalAllyLife += getCurrentLifeInGame(target);
  });

  return totalAllyLife;
}

int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  
  int totalEnemyShields = 0;

  unitStatTotalFinder.forEach([&caster, &totalEnemyShields] (const CUnit *target) {
    if (target->status & UnitStatus::Invincible)
      return;

    if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return;

    if (!units_dat::ShieldsEnabled[target->id])
      return;

    totalEnemyShields += target->shields / 256;
  });

  return totalEnemyShields;
}

int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);

  int totalEnemyEnergy = 0;

  unitStatTotalFinder.forEach([&caster, &totalEnemyEnergy] (const CUnit *target) {
    if (target->status & UnitStatus::Invincible)
      return;

    if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return;

    if (!target->isValidCaster())
      return;

    totalEnemyEnergy += target->energy / 256;
  });

  return totalEnemyEnergy;
}

int getTotalEnemyNukeValueInArea(int x, int y, int searchBounds, const CUnit *caster) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  
  int totalNukeTargetValue = 0;

  unitStatTotalFinder.forEach([&caster, &totalNukeTargetValue] (const CUnit *target) {
    if (target->status & UnitStatus::Invincible)
      return;

    if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return;

    if ((units_dat::BaseProperty[target->id] & UnitProperty::Worker)
        || !(target->status & UnitStatus::IsBuilding))
      totalNukeTargetValue += getCurrentLifeInGame(target);

    if (units_dat::BaseProperty[target->id] & UnitProperty::Building) {
      if (target->canDetect()
          || target->id == UnitId::sunken_colony
          || target->id == UnitId::lurker)
        totalNukeTargetValue = 800;  //Any static defense is at least 800 value
    }
  });

  return totalNukeTargetValue;
}

} //AI
