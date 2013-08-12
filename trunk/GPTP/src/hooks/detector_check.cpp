#include "detector_check.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"

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
