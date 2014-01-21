#include "emp_shockwave.h"

namespace AI {

class EmpShockwaveTargetFinderShieldProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    EmpShockwaveTargetFinderShieldProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

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
    }
};

class EmpShockwaveTargetFinderEnergyProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    EmpShockwaveTargetFinderEnergyProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
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
    }
};

CUnit* findBestEmpShockwaveTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  CUnit *result = scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    EmpShockwaveTargetFinderShieldProc(caster, isUnderAttack));

  if (result || isUnderAttack)
    return result;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    EmpShockwaveTargetFinderEnergyProc(caster, isUnderAttack));
}

} //AI
