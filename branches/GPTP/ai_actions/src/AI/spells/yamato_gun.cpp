#include "yamato_gun.h"

namespace AI {

class YamatoGunTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    YamatoGunTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
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
          && !(Unit::BaseProperty[target->id] & UnitProperty::Hero))
        return true;

      return false;
    }
};

CUnit* findBestYamatoGunTarget(const CUnit *caster, bool isUnderAttack) {
  if (isUnderAttack) {
    int bounds = 32 * 16;
    return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
      caster->getX() - bounds, caster->getY() - bounds,
      caster->getX() + bounds, caster->getY() + bounds,
      YamatoGunTargetFinderProc(caster, isUnderAttack));
  }
  else {
    return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
      YamatoGunTargetFinderProc(caster, isUnderAttack));
  }
}

} //AI
