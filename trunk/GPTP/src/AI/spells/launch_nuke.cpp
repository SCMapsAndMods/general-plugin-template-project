#include "psi_storm.h"

namespace AI {

CUnit* findBestNukeLaunchTarget(const CUnit *caster, bool isUnderAttack) {
  auto nukeLaunchTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (target == caster)
      return false;

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

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    nukeLaunchTargetFinder);
}

} //AI
