#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestPsiStormTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else if (isUmsMode(caster->playerId))
    bounds = 32 * 64;
  else
    bounds = 32 * 32;

  auto psiStormTargetFinder = [&caster, &isUnderAttack] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::PsiStorm, target, caster))
      return false;

    const int totalEnemyLife = getTotalEnemyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::PsiStorm);
    if (!isUnderAttack && totalEnemyLife < 250)
      return false;

    const int totalAllyLife = getTotalAllyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::PsiStorm);
    if (totalAllyLife * 2 >= totalEnemyLife)
      return false;

    return true;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, psiStormTargetFinder);
}

} //AI
