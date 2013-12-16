#include "spawn_broodlings.h"

namespace AI {

class SpawnBroodlingsTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    SpawnBroodlingsTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (!isTargetWorthHitting(target, caster))
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::SpawnBroodlings, target, caster))
        return false;

      if (Unit::BaseProperty[target->id] & UnitProperty::Hero)
        return false;

      if (!isUnderAttack && getCurrentLifeInGame(target) < 100)
        return false;

      if (Unit::BaseProperty[target->id] & UnitProperty::Worker
          || target->id == UnitId::siege_tank
          || target->id == UnitId::siege_tank_s
          || target->id == UnitId::medic
          || (target->mainOrderId == OrderId::NukeWait
              || target->mainOrderId == OrderId::NukeTrack))
        return true;

      return false;
    }
};

CUnit* findBestSpawnBroodlingsTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    SpawnBroodlingsTargetFinderProc(caster, isUnderAttack));
}

} //AI
