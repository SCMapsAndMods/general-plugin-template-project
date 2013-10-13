#include "detector.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"

namespace hooks {

/// Determines whether this unit can detect cloaked/burrowed units.
/// This overrides the EXE edit settings for Detectors in FireGraft
bool unitCanDetectHook(const CUnit *unit) {
  //Default StarCraft behavior
  return Unit::BaseProperty[unit->id] & UnitProperty::Detector
         && unit->status & UnitStatus::Completed    // Is completed
         && !(unit->status & UnitStatus::Disabled)  // Is not disabled/unpowered
         && !unit->lockdownTimer
         && !unit->stasisTimer
         && !unit->maelstromTimer
         && !unit->isBlind;
}

//Check if the @p unit can see the @p target (assuming target is cloaked).
u32 getCloakedTargetVisibility(const CUnit *unit, const CUnit* target) {
  //Default StarCraft behavior
  if (target->status & UnitStatus::IsHallucination)
    return 0;

  if (unitCanDetectHook(unit) && unit != target) {
    if ((1 << unit->playerId) & target->sprite->visibilityFlags) {
      u32 detectionRange;

      if (unit->status & UnitStatus::GroundedBuilding)
        detectionRange = 224;
      else
        detectionRange = 32 * unit->getSightRange();

      if (unit->getDistanceToTarget(target) <= detectionRange)
        return ((1 << unit->playerId) | playerVision->flags[unit->playerId] | unit->parasiteFlags);
    }
  }

  return 0;
}

} //hooks