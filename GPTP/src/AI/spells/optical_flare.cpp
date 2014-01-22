#include "psi_storm.h"

namespace AI {

CUnit* findBestOpticalFlareTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 32;

  auto opticalFlareTargetFinder = [&caster] (const CUnit *target) -> bool {
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
  };

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    opticalFlareTargetFinder);
}

} //AI
