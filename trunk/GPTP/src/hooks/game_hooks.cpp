/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../graphics/graphics.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/ExtendSightLimit.h"
#include <cstdio>

bool firstRun = true;

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
	if (!scbw::isGamePaused()) { //If the game is not paused
    graphics::resetAllGraphics();

		if (firstRun) {
			scbw::printText("Hello, world!");
			firstRun = false;
		}

		// Loop through the unit table.
    // Warning: There is no guarantee that the current unit is actually a unit
    // rather than an unused space in memory.
		//for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
		//	CUnit* unit = &unitTable[i];
    //  //Write your code here
		//}

    // Alternative looping method
    // Guarantees that [unit] points to an actual unit.
    for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->next) {
      //Write your code here
    }

    for (int i = 0; i < 12 && i < *clientSelectionCount; ++i) {
      CUnit *selectedUnit = clientSelectionGroup[i];
      char buffer[10];
      sprintf_s(buffer, 10, "%d HP", selectedUnit->hitPoints >> 8);
      int yOffset = 10 + scbw::getUnitOverlayAdjustment(selectedUnit) * 10;
      graphics::drawTextOnMap(selectedUnit->getX(), selectedUnit->getY() - yOffset, std::string(buffer));

      if (selectedUnit->orderTarget.unit) {
        CUnit *target = selectedUnit->orderTarget.unit;
        graphics::drawLineOnMap(selectedUnit->getX(), selectedUnit->getY(),
                                target->getX(), target->getY(), graphics::RED);
        graphics::drawCircleOnMap(target->getX(), target->getY(), 8, graphics::RED);
      }
      else {
        const int targX = selectedUnit->orderTarget.pt.x, targY = selectedUnit->orderTarget.pt.y;
        if (targX != 0 && targY != 0) {
          graphics::drawLineOnMap(selectedUnit->getX(), selectedUnit->getY(),
                                  targX, targY, graphics::YELLOW);
          graphics::drawFilledCircleOnMap(targX, targY, 5, graphics::YELLOW);
        }
      }

      //Show Siege Tank attack radius
      if (selectedUnit->id == UnitId::siege_tank_s)
        graphics::drawCircleOnMap(selectedUnit->getX(), selectedUnit->getY(),
                                  12 * 32 + 32, graphics::CYAN);
    }

    if (*clientSelectionCount == 1) {
      CUnit *unit = clientSelectionGroup[0];
      char buffer[1000];

      graphics::drawBoxOnScreen(10, 10, 170, 160, graphics::BLACK);
      graphics::drawFilledBoxOnScreen(11, 11, 169, 159, graphics::GREY);
      graphics::drawTextOnScreen(20, 20, std::string("UNIT STATUS:"));

      sprintf_s(buffer, 1000, "Position: (%d, %d)\nHP: %d\nShield: %d\nGround cooldown: %d\nAir cooldown: %d",
        unit->getX(), unit->getY(), unit->hitPoints >> 8, unit->shields >> 8, unit->groundWeaponCooldown, unit->airWeaponCooldown);
      graphics::drawTextOnScreen(30, 40, std::string(buffer));
    }

		// Loop through the bullet table.
    // Warning: There is no guarantee that the current bullet is actually a
    // bullet rather than an unused space in memory
		//for (int i = 0; i < BULLET_ARRAY_LENGTH; ++i) {
		//	BULLET* bullet = &bulletTable[i];
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
