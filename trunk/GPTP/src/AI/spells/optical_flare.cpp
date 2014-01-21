#include "psi_storm.h"

namespace AI {

class OpticalFlareTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    OpticalFlareTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (!isTargetWorthHitting(target, caster))
        return false;

      if (units_dat::BaseProperty[target->id] & UnitProperty::Building)
        return false;

      if (target->isBlind)
        return false;

      if (target->canDetect())
        return true;

      if (getCurrentLifeInGame(target) > 80)
        return true;

      return false;
    }
};

CUnit* findBestOpticalFlareTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 32;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    OpticalFlareTargetFinderProc(caster, isUnderAttack));
}

} //AI
