#include "spells.h"
#include <AI/ai_common.h>

namespace AI {

CUnit* findBestYamatoGunTarget(const CUnit *caster, bool isUnderAttack) {
  auto yamatoGunTargetFinder = [&caster, &isUnderAttack] (const CUnit *target) -> bool {
    if (!isTargetWorthHitting(target, caster))
      return false;

    if ((target->status & UnitStatus::GroundedBuilding)
        && unitCanAttack(target))
      return true;

    if (target->id == UnitId::bunker && target->hasLoadedUnit())
      return true;

    if (target->id == UnitId::egg)
      return false;

    const int targetLife = getCurrentLifeInGame(target);
    if (200 <= targetLife && targetLife <= 450
        && !(units_dat::BaseProperty[target->id] & UnitProperty::Hero))
      return true;

    return false;
  };

  if (isUnderAttack) {
    int bounds = 32 * 16;
    return scbw::UnitFinder::getNearestTarget(
      caster->getX() - bounds, caster->getY() - bounds,
      caster->getX() + bounds, caster->getY() + bounds,
      caster, yamatoGunTargetFinder);
  }
  else {
    return scbw::UnitFinder::getNearestTarget(caster, yamatoGunTargetFinder);
  }
}

} //AI
