#include "lockdown.h"
#include <SCBW/api.h>
#include <algorithm>

namespace weapon_hooks {

void lockdownHook(CUnit *unit, timer) {
  //Default StarCraft behavior

  if (unit->lockdownTimer == 0) {
    CUnit const *overlayUnit = unit->subunit ? unit->subunit : unit;

    overlayUnit->sprite->createTopOverlay(ImageId::LockdownField_Small
      + scbw::getUnitOverlayAdjustment(unit));
  }

  unit->lockdownTimer = std::max(unit->lockdownTimer, timer);
  //freezeUnit(unit);
}

} //weapon_hooks
