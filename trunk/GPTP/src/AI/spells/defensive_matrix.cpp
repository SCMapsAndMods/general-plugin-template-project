#include "spells.h"
#include <AI/ai_common.h>
#include <hooks/tech_target_check.h>

namespace AI {

CUnit* findBestDefensiveMatrixTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  auto defensiveMatrixTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (target == caster)
      return false;

    if (target->defensiveMatrixHp)
      return false;

    if (!scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
      return false;

    if (!target->orderTarget.unit)  //Ignore idle units?
      return false;

    if (target->status & UnitStatus::RequiresDetection)
      return false;

    if (getMaxHpInGame(target) < 100 || getCurrentHpInGame(target) > 75)
      return false;

    if (target->status & UnitStatus::Invincible)
      return false;

    if (hooks::getTechUseErrorMessage(target, caster->playerId, TechId::DefensiveMatrix) != 0)
      return false;

    return true;
  };

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    defensiveMatrixTargetFinder);
}

} //AI
