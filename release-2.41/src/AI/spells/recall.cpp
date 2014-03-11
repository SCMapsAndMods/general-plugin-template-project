#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestRecallTarget(const CUnit *caster, bool isUnderAttack) {
  if (isUnitInUnsafeRegion(caster))
    return nullptr;

  auto recallTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (target->playerId != caster->playerId)
      return false;

    if (!target->orderTarget.unit)
      return false;

    if (target->id == UnitId::carrier
        || target->id == UnitId::gantrithor
        || target->id == UnitId::reaver
        || target->id == UnitId::warbringer) {
      if (target->getCurrentShieldsInGame() <= 10)
        return true;
    }

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(caster, recallTargetFinder);
}

} //AI
