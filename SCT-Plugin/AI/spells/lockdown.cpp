#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestLockdownTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else if (isUmsMode(caster->playerId))
    bounds = 32 * 64;
  else
    bounds = 32 * 32;

  auto lockdownTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::Lockdown, target, caster))
      return false;

    if (target->isFrozen())
      return false;

    if (target->id == UnitId::dropship || target->id == UnitId::shuttle) {
      if (target->movementFlags & 0x2 && target->hasLoadedUnit())
        return true;
    }

    if (target->id == UnitId::carrier || target->id == UnitId::reaver)
      return true;

    if (isTargetAttackingAlly(target, caster))
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, lockdownTargetFinder);
}

} //AI
