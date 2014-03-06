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
    //Cast Ocular Implants only on friendly units
    if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return false;

    if (units_dat::BaseProperty[target->id] & UnitProperty::Building)
      return false;

    if (target->isBlind)
      return false;

    if (target->canDetect())
      return true;

    if (getCurrentLifeInGame(target) > 80)
      return true;

    //Cast Ocular Implants only on expensive units
    if (units_dat::DestroyScore[target->id] >= 300)
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, opticalFlareTargetFinder);
}

} //AI
