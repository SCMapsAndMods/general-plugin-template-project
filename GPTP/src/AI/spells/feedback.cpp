#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestFeedbackTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else if (isUmsMode(caster->playerId))
    bounds = 32 * 64;
  else
    bounds = 32 * 32;

  auto feedbackTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (!target->isValidCaster())
      return false;

    if (target->status & UnitStatus::GroundedBuilding)
      return false;

    if (units_dat::BaseProperty[target->id] & UnitProperty::Hero)
      return false;

    if (target->energy / 256 >= getCurrentLifeInGame(target))
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, feedbackTargetFinder);
}

} //AI
