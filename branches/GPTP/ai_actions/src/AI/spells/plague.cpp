#include "plague.h"
#include <SCBW/enumerations.h>

namespace AI {

class PlagueTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    PlagueTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (!isTargetWorthHitting(target, caster))
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::Plague, target, caster))
        return false;

      if (target->plagueTimer)
        return false;

      if (Unit::BaseProperty[target->id] & UnitProperty::Hero)
        return false;

      const int totalEnemyLife = getTotalEnemyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::Plague);
      if (!isUnderAttack && totalEnemyLife < 250)
        return false;

      const int totalAllyLife = getTotalAllyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::Plague);
      if (totalAllyLife * 2 >= totalEnemyLife)
        return false;

      return true;
    }
};

CUnit* findBestPlagueTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 8;
  else
    bounds = 32 * 128;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    PlagueTargetFinderProc(caster, isUnderAttack));
}

} //AI
