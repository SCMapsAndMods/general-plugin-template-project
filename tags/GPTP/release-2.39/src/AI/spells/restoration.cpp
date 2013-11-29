#include "psi_storm.h"

namespace AI {

class RestorationTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    RestorationTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (target->playerId != caster->playerId)
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::Restoration, target, caster))
        return false;

      if (getCurrentLifeInGame(target) <= 60)
        return false;

      if (target->ensnareTimer
          || target->plagueTimer
          || target->irradiateTimer
          || target->lockdownTimer
          || target->maelstromTimer
          || target->acidSporeCount)
        return true;

      return false;
    }
};

CUnit* findBestRestorationTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds = 32 * 32;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    RestorationTargetFinderProc(caster, isUnderAttack));
}

class RestorationTargetFinderProcSituational: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    RestorationTargetFinderProcSituational(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (target->playerId != caster->playerId)
        return false;

      if (!(target->parasiteFlags || target->maelstromTimer))
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::Restoration, target, caster))
        return false;

      return true;
    }
};

CUnit* findBestRestorationTargetSituational(const CUnit *caster, bool isUnderAttack) {
  if (isUnderAttack)
    return NULL;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    RestorationTargetFinderProcSituational(caster, isUnderAttack));
}
} //AI
