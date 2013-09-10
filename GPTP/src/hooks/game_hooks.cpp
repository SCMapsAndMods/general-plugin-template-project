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
    //  CUnit* unit = &unitTable[i];
    //  //Write your code here
    //}

    // Alternative looping method
    // Guarantees that [unit] points to an actual unit.
    for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->next) {
      //Write your code here
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

    if (*clientSelectionCount == 1) {
      CUnit *carrier = clientSelectionGroup[0];
      if (carrier->id == UnitId::carrier) {
        graphics::drawFilledBoxOnScreen(10, 10, 300, 200, graphics::BLACK);
        char buffer[1000];
        sprintf(buffer, "Carrier order: %d (s=%d) / secondary %d (s=%d)\nInterceptor count: %d outside, %d inside\n",
                carrier->mainOrderId,
                carrier->mainOrderTimer,
                carrier->secondaryOrderId,
                carrier->secondaryOrderState,
                carrier->carrier.outHangarCount,
                carrier->carrier.inHangarCount
                );
        for (CUnit *i = carrier->carrier.inHangarChild; i; i = i->interceptor.prev) {
          char buffer2[100];
          sprintf(buffer2, " - In: %#X [p=%#X, n=%#X], %d HP, %d SP\n",
                  i, i->interceptor.prev, i->interceptor.next,
                  i->hitPoints >> 8, i->shields >> 8);
          strcat(buffer, buffer2);                  
        }
        for (CUnit *i = carrier->carrier.outHangarChild; i; i = i->interceptor.prev) {
          char buffer2[100];
          sprintf(buffer2, " - Out: %#X [p=%#X, n=%#X], %d HP, %d SP\n",
                  i, i->interceptor.prev, i->interceptor.next,
                  i->hitPoints >> 8, i->shields >> 8);
          strcat(buffer, buffer2);                  
        }
        graphics::drawTextOnScreen(20, 20, std::string(buffer));
      }
    }
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
