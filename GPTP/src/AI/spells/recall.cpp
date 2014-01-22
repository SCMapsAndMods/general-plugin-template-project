#include "psi_storm.h"

namespace AI {

CUnit* findBestRecallTarget(const CUnit *caster, bool isUnderAttack) {
  if (isUnitInUnsafeRegion(caster))
    return nullptr;

  auto recallTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (target == caster)
      return false;

    if (target->playerId != caster->playerId)
      return false;

    if (!target->orderTarget.unit)
      return false;

    if (target->id == UnitId::carrier
        || target->id == UnitId::gantrithor
        || target->id == UnitId::reaver
        || target->id == UnitId::warbringer) {
      if (target->shields / 256 <= 10)
        return true;
    }

    return false;
  };

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    recallTargetFinder);
}

} //AI
