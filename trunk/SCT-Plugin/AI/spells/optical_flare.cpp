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

      //Cast Ocular Implants only on friendly units
      if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
        return false;

      if (Unit::BaseProperty[target->id] & UnitProperty::Building)
        return false;

      if (target->isBlind)
        return false;

      if (target->canDetect())
        return true;

      //Cast Ocular Implants only on expensive units
      if (Unit::DestroyScore[target->id] >= 300)
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
