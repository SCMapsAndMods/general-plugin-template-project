#include "ai_common.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include "../SCBW/UnitFinder.h"

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

//-------- Find best spell target --------//

class BestSpellTargetFinderProc {
  private:
    const CUnit *caster;
    scbw::UnitFinderCallbackMatchInterface &callback;
  public:
    int operator()(const CUnit *target) {
      if (callback.match(target))
        return 1;
    }
};

const CUnit* findBestSpellTarget(int x, int y, int searchBounds,
                                 scbw::UnitFinderCallbackMatchInterface &callback) {
  static scbw::UnitFinder spellTargetFinder;
  spellTargetFinder.search(x - searchBounds, y - searchBounds,
                           x + searchBounds, y + searchBounds);
  //return spellTargetFinder.getBest(
  return NULL;
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

      sum += (target->hitPoints + 255) / 256;
      if (weaponId != WeaponId::Plague && Unit::ShieldsEnabled[target->id])
        sum += target->shields / 256;
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

      sum += (target->hitPoints + 255) / 256;
      if (Unit::ShieldsEnabled[target->id])
        sum += target->shields / 256;
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
