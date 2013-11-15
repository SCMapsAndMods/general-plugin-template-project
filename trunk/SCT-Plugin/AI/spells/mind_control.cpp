#include "psi_storm.h"

namespace AI {

class MindControlTargetFinderProc: public scbw::UnitFinderCallbackMatchInterface {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    MindControlTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool match(const CUnit *target) {
      if (target == caster)
        return false;

      //Air units cannot be Mind Controlled
      if (target->status & UnitStatus::InAir)
        return false;

      if (!isTargetWorthHitting(target, caster))
        return false;

      if (Unit::BaseProperty[target->id] & UnitProperty::Hero)
        return false;

      //if (target->id == UnitId::shuttle || target->id == UnitId::dropship)
      //  if (target->hasLoadedUnit())
      //    return true;

      switch (target->id) {
        //case UnitId::shuttle:
        //case UnitId::dropship:
        //  if (target->hasLoadedUnit())
        //    return true;
        //  break;

        case UnitId::siege_tank:
        case UnitId::siege_tank_s:
        //case UnitId::science_vessel:
        //case UnitId::battlecruiser:
        case UnitId::medic:
        case UnitId::ghost: //Added
        //case UnitId::overlord:
        case UnitId::ultralisk:
        //case UnitId::queen:
        case UnitId::defiler:
        //case UnitId::devourer:
        case UnitId::dark_archon:
        case UnitId::high_templar:
        //case UnitId::arbiter:
        //case UnitId::carrier:
        case UnitId::reaver:
        case UnitId::lurker:
          return true;

        default:
          if (Unit::MineralCost[target->id] >= Unit::MineralCost[UnitId::dark_templar]
              && Unit::GasCost[target->id] >= Unit::GasCost[UnitId::dark_templar])
            return true;
          break;
      }

      return false;
    }
};

CUnit* findBestMindControlTarget(const CUnit *caster, bool isUnderAttack) {
  int bounds = 32 * 32;

  return scbw::UnitFinder::getNearest(caster->getX(), caster->getY(),
    caster->getX() - bounds, caster->getY() - bounds,
    caster->getX() + bounds, caster->getY() + bounds,
    MindControlTargetFinderProc(caster, isUnderAttack));
}

} //AI
