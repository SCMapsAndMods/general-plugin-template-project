#include "defensive_matrix.h"
#include <hooks/tech_target_check.h>

namespace AI {

class DefensiveMatrixTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    DefensiveMatrixTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
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
    }
};

CUnit* findBestDefensiveMatrixTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds;
  if (isUnderAttack)
    bounds = 32 * 9;
  else
    bounds = 32 * 64;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    DefensiveMatrixTargetFinderProc(caster, isUnderAttack));
}

} //AI
