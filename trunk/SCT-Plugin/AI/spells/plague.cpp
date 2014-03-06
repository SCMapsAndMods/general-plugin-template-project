#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestPlagueTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 128;

  auto plagueTargetFinder = [&caster, &isUnderAttack] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::Plague, target, caster))
      return false;

    if (target->plagueTimer)
      return false;

    if (units_dat::BaseProperty[target->id] & UnitProperty::Hero)
      return false;

    const int totalEnemyLife = getTotalEnemyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::Plague);
    if (!isUnderAttack && totalEnemyLife < 250)
      return false;

    const int totalAllyLife = getTotalAllyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::Plague);
    if (totalAllyLife * 2 >= totalEnemyLife)
      return false;

    return true;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, plagueTargetFinder);
}

} //AI
