#include "psi_storm.h"

namespace AI {

class ParasiteTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    ParasiteTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (!isTargetWorthHitting(target, caster))
        return false;

      if (isUmsMode(caster->playerId) && target->parasiteFlags)
        return false;

      if (target->parasiteFlags & (1 << caster->playerId))
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::Parasite, target, caster))
        return false;

      if (target->canDetect())
        return true;

      if (!(target->status & UnitStatus::IsHallucination)
          && (target->id != UnitId::overlord || scbw::getUpgradeLevel(target->playerId, UpgradeId::VentralSacs))
          && Unit::SpaceProvided[target->id] > 0)
        return true;

      if (target->isValidCaster())
        return true;

      if (Unit::BaseProperty[target->id] & UnitProperty::Worker)
        return true;

      if (getCurrentLifeInGame(target) >= 300)
        return true;

      return false;
    }
};

CUnit* findBestParasiteTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    ParasiteTargetFinderProc(caster, isUnderAttack));
}

} //AI
