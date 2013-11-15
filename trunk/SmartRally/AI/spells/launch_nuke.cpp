#include "psi_storm.h"

namespace AI {

class NukeLaunchTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    NukeLaunchTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if ((target->status & (UnitStatus::Cloaked | UnitStatus::RequiresDetection))
          && !target->isVisibleTo(caster->playerId))
        return false;

      if (scbw::isAlliedTo(caster->playerId, target->getLastOwnerId()))
        return false;

      const int totalEnemyClumpValue = getTotalEnemyNukeValueInArea(target->getX(), target->getY(), 192, caster);
      if (totalEnemyClumpValue >= 800)
        return true;

      return false;
    }
};

CUnit* findBestNukeLaunchTarget(const CUnit *caster, bool isUnderAttack) {
  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    NukeLaunchTargetFinderProc(caster, isUnderAttack));
}

} //AI
