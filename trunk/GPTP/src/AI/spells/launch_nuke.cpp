#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestNukeLaunchTarget(const CUnit *caster, bool isUnderAttack) {
  auto nukeLaunchTargetFinder = [&caster] (const CUnit *target) -> bool {
    if ((target->status & (UnitStatus::Cloaked | UnitStatus::RequiresDetection))
        && !target->isVisibleTo(caster->playerId))
      return false;

    if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return false;

    const int totalEnemyClumpValue = getTotalEnemyNukeValueInArea(target->getX(), target->getY(), 192, caster);
    if (totalEnemyClumpValue >= 800)
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(caster, nukeLaunchTargetFinder);
}

} //AI
