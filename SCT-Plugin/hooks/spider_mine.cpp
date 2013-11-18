#include "spider_mine.h"
#include <SCBW/api.h>
#include <SCBW/enumerations.h>
#include <SCBW/UnitFinder.h>

//Helper class for findBestSpiderMineTargetHook()
class SpiderMineTargetFinder: public scbw::UnitFinderCallbackMatchInterface {
  const CUnit *spiderMine;
  public:
    //Check if @p unit is a suitable target for the @p spiderMine.
    bool match(const CUnit *unit) {
      //Default StarCraft behavior

      //Don't attack allied units
      if (scbw::isAlliedTo(spiderMine->playerId, unit->getLastOwnerId()))
        return false;

      //Don't attack invincible units / air units / buildings
      using UnitStatus::Invincible;
      using UnitStatus::InAir;
      using UnitStatus::GroundedBuilding;
      if (unit->status & (Invincible | InAir | GroundedBuilding))
        return false;

      //Don't attack hovering units
      if (Unit::MovementFlags[unit->id] == (0x01 | 0x40 | 0x80))  //Note: This is not a mistake; SC actually uses a "==" comparison to check flags (I know it's a WTF).
        return false;

      return true;
    }

    //Constructor
    SpiderMineTargetFinder(const CUnit *spiderMine) : spiderMine(spiderMine) {}
};

namespace hooks {

//Return the best target for the Spider Mine. If there is no suitable target,
//return NULL instead.
CUnit* findBestSpiderMineTargetHook(const CUnit *mine) {
  //Default StarCraft behavior

  //Don't search for a target if the spider mine is under a Disruption Web
  if (mine->status & UnitStatus::CanNotAttack)
    return NULL;

  s32 range = 32 * mine->getSeekRange();
  
  return scbw::UnitFinder::getNearest(
    mine->getX(), mine->getY(),
    mine->getX() - range, mine->getY() - range,
    mine->getX() + range, mine->getY() + range,
    SpiderMineTargetFinder(mine));
}

//Return the initial burrowing delay time (in frames) for the Spider Mine.
u8 getSpiderMineBurrowTimeHook(const CUnit *spiderMine) {
  //Default StarCraft behavior
  return 60;
}

} //hooks
