#include "psi_storm.h"

namespace AI {

class RecallTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    RecallTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      if (target->playerId != caster->playerId)
        return false;

      if (!target->orderTarget.unit)
        return false;

      if (target->id == UnitId::carrier
          || target->id == UnitId::gantrithor
          || target->id == UnitId::reaver
          || target->id == UnitId::warbringer) {
        if (target->shields / 256 <= 10)
          return true;
      }

      return false;
    }
};

CUnit* findBestRecallTarget(const CUnit *caster, bool isUnderAttack) {
  if (isUnitInUnsafeRegion(caster))
    return NULL;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    RecallTargetFinderProc(caster, isUnderAttack));
}

} //AI
