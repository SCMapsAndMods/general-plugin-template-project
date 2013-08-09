/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/ExtendSightLimit.h"
#include <cstdio>

bool firstRun = true;

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
	if (!*IS_GAME_PAUSED) { //If the game is not paused

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
      char buffer[500];
      if (unit->id == UnitId::zergling) {
        if (unit->ensnareTimer)
          sprintf(buffer, "Ensnared zergling cool: %d", unit->groundWeaponCooldown);
        else
          sprintf(buffer, "Clean zergling cool: %d", unit->groundWeaponCooldown);

        scbw::printText(buffer);
      }
      //Write your code here
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
