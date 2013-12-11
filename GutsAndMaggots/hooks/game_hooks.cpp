/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include <graphics/graphics.h>
#include <SCBW/api.h>
#include <SCBW/scbwdata.h>
#include <SCBW/ExtendSightLimit.h>
#include "psi_field.h"
#include <cstdio>

bool firstRun = true;
//자원 제공 및 텍스트 효과를 위한 타이머
u32 extractorTimer;

namespace hooks {

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
  if (!scbw::isGamePaused()) { //If the game is not paused
    graphics::resetAllGraphics();
    hooks::updatePsiFieldProviders();

    if (firstRun) {
      scbw::printText("Hello, world!");
      firstRun = false;
      extractorTimer = 0;
    }

    // Loop through the unit table.
    // Warning: There is no guarantee that the current unit is actually a unit
    // rather than an unused space in memory.
    //for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
    //  CUnit* unit = &unitTable[i];
    //  //Write your code here
    //}

    ++extractorTimer;
    u32 extractorTimerTick = extractorTimer % 15; //1초 == 15프레임

    // Alternative looping method
    // Guarantees that [unit] points to an actual unit.
    for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->next) {
      //익스트랙터에서 매초마다 자원 제공
      if (unit->id == UnitId::extractor && unit->status & UnitStatus::Completed)
      {
        if (extractorTimerTick == 0) {
          resources->minerals[unit->playerId]           += EXTRACTOR_MINERALS;
          resources->gas[unit->playerId]                += EXTRACTOR_GAS;
          resources->cumulativeMinerals[unit->playerId] += EXTRACTOR_MINERALS;
          resources->cumulativeGas[unit->playerId]      += EXTRACTOR_GAS;
        }

        //텍스트 표시: 나타나서 천천히 위로 떠오르게        
        const char extractorResourceText[] = "\x1F미네랄 +" STR(EXTRACTOR_MINERALS) " \x07가스 +" STR(EXTRACTOR_GAS);
        
        int textX = unit->getX() - 40;
        int textY = unit->getY() - 30 - extractorTimerTick / 2;

        graphics::drawText(textX, textY, extractorResourceText,
          graphics::FONT_MEDIUM, graphics::ON_MAP);
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
