#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestRestorationTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds = 32 * 32;

  auto restorationTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (target == caster)
      return false;

    if (target->playerId != caster->playerId)
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::Restoration, target, caster))
      return false;

    if (getCurrentLifeInGame(target) <= 60)
      return false;

    if (target->ensnareTimer
        || target->plagueTimer
        || target->irradiateTimer
        || target->lockdownTimer
        || target->maelstromTimer
        || target->acidSporeCount)
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    restorationTargetFinder);
}

CUnit* findBestRestorationTargetSituational(const CUnit *caster, bool isUnderAttack) {
  if (isUnderAttack)
    return nullptr;

  auto restorationTargetFinderSituational = [&caster] (const CUnit *target) -> bool {
    if (target == caster)
      return false;

    if (target->playerId != caster->playerId)
      return false;

    if (!(target->parasiteFlags || target->maelstromTimer))
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::Restoration, target, caster))
      return false;

    return true;
  };

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    restorationTargetFinderSituational);
}

} //AI
