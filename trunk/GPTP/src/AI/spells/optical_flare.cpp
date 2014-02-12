#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestOpticalFlareTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 32;

  auto opticalFlareTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (units_dat::BaseProperty[target->id] & UnitProperty::Building)
      return false;

    if (target->isBlind)
      return false;

    if (target->canDetect())
      return true;

    if (target->getCurrentLifeInGame() > 80)
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, opticalFlareTargetFinder);
}

} //AI
