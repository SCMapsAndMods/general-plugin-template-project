#include "spider_mine.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"


namespace hooks {

//Return the initial burrowing delay time (in frames) for the Spider Mine.
u8 getSpiderMineBurrowTimeHook(const CUnit *spiderMine) {
  //Default StarCraft behavior
  return 60;
}

//Decide if @p spiderMine can attack the @p target.
bool spiderMineCanTargetUnitHook(const CUnit *target, const CUnit *spiderMine) {
  //Default StarCraft behavior
  s8 targetOwner = target->playerId;
  if (targetOwner == 11)
    targetOwner = target->sprite->playerID;

  //Don't attack allied units
  if (playerAlliance[spiderMine->playerId].flags[targetOwner] != 0)
    return false;

  //Don't attack invincible units / air units / buildings
  if (target->status & (UnitStatus::Invincible | UnitStatus::InAir | UnitStatus::GroundedBuilding))
    return false;

  //Don't attack hovering units
  if (Unit::MovementFlags[target->id] == (0x01 | 0x40 | 0x80))  //Note: This is not a mistake; SC actually uses a "==" comparison to check flags (I know it's a WTF).
    return false;

  return true;
}

} //hooks
