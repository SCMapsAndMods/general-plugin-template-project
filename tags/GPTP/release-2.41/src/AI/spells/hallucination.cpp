#include "spells.h"
#include <AI/ai_common.h>
#include <hooks/tech_target_check.h>

namespace AI {

CUnit* findBestHallucinationTarget(const CUnit *caster, bool isUnderAttack) {
  if (isUnderAttack)
    return nullptr;

  int bounds = 32 * 32;

  auto hallucinationTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (target->playerId != caster->playerId)
      return false;

    if (target->status & UnitStatus::Invincible)
      return false;

    if (hooks::getTechUseErrorMessage(target, caster->playerId, TechId::Hallucination) != 0)
      return false;

    if (target->id == UnitId::carrier
        || target->id == UnitId::scout
        || target->id == UnitId::arbiter
        || target->id == UnitId::archon)
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, hallucinationTargetFinder);
}

} //AI
