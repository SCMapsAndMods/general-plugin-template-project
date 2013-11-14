/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../graphics/graphics.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/ExtendSightLimit.h"
#include "psi_field.h"
#include <cstdio>

bool firstRun = true;

namespace hooks {

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
  if (!scbw::isGamePaused()) { //If the game is not paused
    graphics::resetAllGraphics();
    hooks::updatePsiFieldProviders();

    if (firstRun) {
      //scbw::printText("Hello, world!");
      firstRun = false;
    }

    // Loop through the unit table.
    // Warning: There is no guarantee that the current unit is actually a unit
    // rather than an unused space in memory.
    //for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
    //  CUnit* unit = &unitTable[i];
    //  //Write your code here
    //}

    // Alternative looping method
    // Guarantees that [unit] points to an actual unit.
    for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->next) {
      //Write your code here

      //Set nexus color
      if (unit->id == UnitId::nexus
          && scbw::getUpgradeLevel(unit->playerId, UPGRADE_PARTICLE_FILTER))
      {
        CImage *nexusAttackOverlay = unit->getOverlay(IMAGE_NEXUS_ATTACK_OVERLAY);
        if (nexusAttackOverlay)
          nexusAttackOverlay->coloringData = colorShift[ColorRemapping::GFire].data;
      }
    }

    //Draw attack radius circles for Siege Mode Tanks in current selection
    for (int i = 0; i < *clientSelectionCount; ++i) {
      CUnit *selUnit = clientSelectionGroup->unit[i];
      if (selUnit->id == UnitId::siege_tank_s) {
        graphics::drawCircle(selUnit->getX(), selUnit->getY(),
          selUnit->getMaxWeaponRange(Unit::GroundWeapon[selUnit->subunit->id]) + 30,
          graphics::TEAL, graphics::ON_MAP);
      }
    }

    // Loop through the bullet table.
    // Warning: There is no guarantee that the current bullet is actually a
    // bullet rather than an unused space in memory
    //for (int i = 0; i < BULLET_ARRAY_LENGTH; ++i) {
    //  BULLET* bullet = &bulletTable[i];
    //  //Write your code here
    //}

    // Alternative looping method
    // Guarantees that [bullet] points to an actual bullet.
    //for (BULLET* bullet = *firstBullet; bullet; bullet = bullet->next) {
    //  //Write your code here
    //}
  }
  return true;
}

bool gameOn() {
  firstRun = true;
  return true;
}

bool gameEnd() {
  return true;
}

} //hooks
