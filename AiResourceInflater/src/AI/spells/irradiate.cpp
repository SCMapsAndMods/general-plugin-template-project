#include "psi_storm.h"

namespace AI {

class IrradiateTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    IrradiateTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (!isTargetWorthHitting(target, caster))
        return false;

      if (target->irradiateTimer)
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::Irradiate, target, caster))
        return false;

      if (!(Unit::BaseProperty[target->id] & UnitProperty::Organic))
        return false;

      if (Unit::BaseProperty[target->id] & UnitProperty::Building)
        return false;

      if (target->id == UnitId::larva
          || target->id == UnitId::egg
          || target->id == UnitId::ZergLurkerEgg)
        return false;

      if (isUnderAttack || !isUmsMode(caster->playerId))
        return true;

      if (Unit::BaseProperty[target->id] & UnitProperty::Worker)
        return true;

      if (target->id == UnitId::overlord || target->id == UnitId::medic)
        return true;

      return false;
    }
};

CUnit* findBestIrradiateTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    IrradiateTargetFinderProc(caster, isUnderAttack));
}

} //AI
