#include "lockdown.h"
#include <weapon_hooks/weapon_hooks_util.h>
#include <SCBW/api.h>
#include <algorithm>

namespace weapon_hooks {

void lockdownHook(CUnit *unit, u8 timer) {
  //Default StarCraft behavior

  if (unit->lockdownTimer == 0) {
    CUnit *overlayUnit = unit->subunit ? unit->subunit : unit;

    overlayUnit->sprite->createTopOverlay(ImageId::LockdownField_Small
      + scbw::getUnitOverlayAdjustment(unit));
  }

  unit->lockdownTimer = std::max(unit->lockdownTimer, timer);
  freezeUnit(unit);
}

} //weapon_hooks
