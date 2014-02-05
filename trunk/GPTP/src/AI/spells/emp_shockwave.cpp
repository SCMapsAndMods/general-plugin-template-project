#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestEmpShockwaveTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  auto empShieldTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (!units_dat::ShieldsEnabled[target->id])
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::EMP_Shockwave, target, caster))
      return false;

    const int totalEnemyShields = getTotalEnemyShieldsInArea(target->getX(), target->getY(), 160, caster);
    if (totalEnemyShields >= 200)
      return true;

    return false;
  };

  CUnit *result = scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, empShieldTargetFinder);

  if (result || isUnderAttack)
    return result;
  
  auto empEnergyTargetFinder = [&caster] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if (!target->isValidCaster())
      return false;

    if (!scbw::canWeaponTargetUnit(WeaponId::EMP_Shockwave, target, caster))
      return false;

    const int totalEnemyEnergy = getTotalEnemyEnergyInArea(target->getX(), target->getY(), 160, caster);
    if (totalEnemyEnergy >= 200)
      return true;

    return false;
  };

  return scbw::UnitFinder::getNearestTarget(
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    caster, empEnergyTargetFinder);
}

} //AI
