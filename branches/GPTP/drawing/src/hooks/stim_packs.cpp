#include "stim_packs.h"
#include "../SCBW/api.h"

//Helper function
static u32 randBetween(u32 min, u32 max);


void useStimPacksHook(CUnit *unit) {
  //Default StarCraft behavior
  if (unit->hitPoints > 2560) {
    scbw::playSound(randBetween(278, 279), unit);
    unit->damageHp(2560);
    if (unit->stimTimer < 37) {
      unit->stimTimer = 37;
      unit->updateSpeed();
    }
  }
}

bool canUseStimPacksHook(const CUnit *unit) {
  //Default StarCraft behavior
  return unit->hitPoints > 2560;
}


/**** Helper function definitions. Do not change anything below! ****/

static u32* const IS_IN_GAME_LOOP = (u32*) 0x006D11C8;
static u32* const lastRandomNumber = (u32*) 0x0051CA14;

//Logically equivalent to function @ 0x004DC550
static u32 randBetween(u32 min, u32 max) {
  if (*IS_IN_GAME_LOOP) {
    *lastRandomNumber = 22695477 * (*lastRandomNumber) + 1;
    u32 randomNumber = (*lastRandomNumber >> 16) & 0x7FFF;  //Make a number between 0 and 32767
    return min + ((max - min + 1) * randomNumber >> 15);
  }
  else
    return min;
}
