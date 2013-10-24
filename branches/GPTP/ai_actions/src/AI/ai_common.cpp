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

  if (target->id == UnitId::ProtossScarab
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
        && Unit::GroundWeapon[unit->id] != WeaponId::None)
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
    if (Unit::GroundWeapon[unit->id] != WeaponId::None
        || Unit::AirWeapon[unit->id] != WeaponId::None)
      return true;
  }

  return false;
}

bool isUmsMode(s8 playerId) {
  assert(0 <= playerId && playerId < 8);
  return AIScriptController[playerId].AI_Flags.isUseMapSettings;
}

int getCurrentHpInGame(const CUnit *unit) {
  return (unit->hitPoints + 255) / 256;
}

int getCurrentLifeInGame(const CUnit *unit) {
  if (Unit::ShieldsEnabled[unit->id])
    return getCurrentHpInGame(unit) + unit->shields / 256;
  else
    return getCurrentHpInGame(unit);
}

//-------- Unit stat accumulators --------//

class UnitStatSumProc: public scbw::UnitFinderCallbackProcInterface {
  protected:
    const CUnit *caster;
    int sum;
    u8 weaponId;
  public:
    void set(const CUnit *caster, u8 weaponId = WEAPON_TYPE_COUNT) {
      this->caster = caster; this->weaponId = weaponId; sum = 0;
    }
    int getSum() const { return sum; }

    virtual void proc(CUnit *target) = 0;
};

class EnemyLifeSumProc: public UnitStatSumProc {
  public:
    void proc(CUnit *target) {
      if (target == caster)
        return;

      if (target->status & UnitStatus::Invincible)
        return;

      if (!scbw::canWeaponTargetUnit(weaponId, target, caster))
        return;

      if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
        return;

      if (weaponId == WeaponId::Plague)
        sum += getCurrentHpInGame(target);
      else
        sum += getCurrentLifeInGame(target);
    }
};

class AllyLifeSumProc: public UnitStatSumProc {
  public:
    void proc(CUnit *target) {
      if (target == caster)
        return;

      if (target->status & UnitStatus::Invincible)
        return;

      if (!scbw::canWeaponTargetUnit(weaponId, target, caster))
        return;

      if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
        return;

      sum += getCurrentLifeInGame(target);
    }
};

class EnemyShieldsSumProc: public UnitStatSumProc {
  public:
    void proc(CUnit *target) {
      if (target->status & UnitStatus::Invincible)
        return;

      if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
        return;

      if (!Unit::ShieldsEnabled[target->id])
        return;

      sum += target->shields / 256;
    }
};

class EnemyEnergySumProc: public UnitStatSumProc {
  public:
    void proc(CUnit *target) {
      if (target->status & UnitStatus::Invincible)
        return;

      if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
        return;

      if (!(Unit::BaseProperty[target->id] & UnitProperty::Spellcaster))
        return;

      if (target->status & UnitStatus::IsHallucination)
        return;

      sum += target->energy / 256;
    }
};

//-------- Get total unit stats in area --------//

scbw::UnitFinder unitStatTotalFinder;

int getTotalEnemyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  EnemyLifeSumProc sumProc;
  sumProc.set(caster, weaponId);
  unitStatTotalFinder.forEach(sumProc);
  return sumProc.getSum();
}

int getTotalAllyLifeInArea(int x, int y, int searchBounds, const CUnit *caster, u8 weaponId) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  AllyLifeSumProc sumProc;
  sumProc.set(caster, weaponId);
  unitStatTotalFinder.forEach(sumProc);
  return sumProc.getSum();
}

int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  EnemyShieldsSumProc sumProc;
  sumProc.set(caster);
  unitStatTotalFinder.forEach(sumProc);
  return sumProc.getSum();
}

int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  EnemyEnergySumProc sumProc;
  sumProc.set(caster);
  unitStatTotalFinder.forEach(sumProc);
  return sumProc.getSum();
}

} //AI
