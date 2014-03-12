/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include <graphics/graphics.h>
#include <SCBW/api.h>
#include <SCBW/scbwdata.h>
#include <SCBW/ExtendSightLimit.h>
#include <SCBW/UnitFinder.h>
#include <cstdio>

//Shared by several functions in here
scbw::UnitFinder finder;

void scvAutoRepair(CUnit* scv) {
  auto repairTargetFinder = [&] (const CUnit* unit) -> bool {
    if (unit->playerId != scv->playerId)
      return false;

    if (unit->getRace() != RaceId::Terran)
      return false;
      
    if (scv->getDistanceToTarget(unit) > 128) //Repair distance
      return false;

    if (unit->hitPoints >= units_dat::MaxHitPoints[unit->id])
      return false;

    if (unit->stasisTimer)
      return false;

    if (unit->status & UnitStatus::InTransport)
      return false;

    if (!(units_dat::BaseProperty[unit->id] & UnitProperty::Mechanical))
      return false;

    if (!(unit->status & UnitStatus::Completed))
      return false;

    if (!scv->hasPathToUnit(unit))
      return false;

    return true;
  };

  const CUnit *repairTarget = scbw::UnitFinder::getNearestTarget(
    scv->getX() - 200, scv->getY() - 200,
    scv->getX() + 200, scv->getY() + 200,
    scv, repairTargetFinder);

  if (repairTarget)
    scv->orderTo(OrderId::Repair1, repairTarget);
}

//Order signal: 0x10 denotes "completely lowered" state
void manageSupplyDepot(CUnit* depot) {
  //TODO: Add this to GPTP
  CUnit** const currentIscriptFlingy = (CUnit**) 0x6D11F4;
  CUnit** const currentIscriptUnit = (CUnit**) 0x6D11FC;

  //If the depot has finished lowering, allow ground units to pass
  //Placed here so the AI does not go crazy with lower/raise
  if (depot->orderSignal & 0x10 && !(depot->status & UnitStatus::NoCollide)) {
    depot->status |= UnitStatus::NoCollide;
    depot->sprite->elevationLevel = 3;
  }

  //AI raise/lower behavior
  if (depot->mainOrderId == units_dat::ComputerIdleOrder[depot->id]
      && !(depot->status & UnitStatus::NoBrkCodeStart))
  {
    finder.search(depot->getLeft()  - 120, depot->getTop()    - 120,
                  depot->getRight() + 120, depot->getBottom() + 120);
    
    //Check if there is an enemy ground unit nearby
    auto isNearbyEnemyGroundUnit = [&] (const CUnit* unit) -> bool {
      if (!unit || unit == depot)
        return false;
      
      if (unit->status & UnitStatus::InAir)
        return false;

      if (scbw::isAlliedTo(depot->playerId, unit->getLastOwnerId()))
        return false;
      
      return depot->getDistanceToTarget(unit) <= 96;
    };
          
    //Is lowered -> raise
    if (depot->status & UnitStatus::NoCollide) {
      if (finder.getFirst(isNearbyEnemyGroundUnit))
        depot->orderTo(OrderId::Stop);
    }
    //Is raised -> lower
    else {
      if (!finder.getFirst(isNearbyEnemyGroundUnit))
        depot->orderTo(OrderId::Stop);
    }
  }

  //Lower/raise trigger
  if (depot->mainOrderId == OrderId::Stop
      && !(depot->status & UnitStatus::NoBrkCodeStart))
  {
    //Erase Stop order (?)
    depot->orderToIdle();

    //Is lowered -> raise
    if (depot->status & UnitStatus::NoCollide) {
      //Check if there is a ground unit on top of the Supply Depot
      finder.search(depot->getLeft(), depot->getTop(), depot->getRight(), depot->getBottom());
      
      auto unitPreventsRaisingDepot = [&] (const CUnit* unit) {
        return unit && unit != depot && (unit->status & UnitStatus::InAir);
      };

      if (finder.getFirst(unitPreventsRaisingDepot)) {
        //Display error message and stop
        scbw::showErrorMessageWithSfx(depot->playerId, 1572, 2);
      }
      else {
        CUnit* const tempUnit = *currentIscriptUnit;
        CUnit* const tempFlingy = *currentIscriptFlingy;
        *currentIscriptUnit = depot;
        *currentIscriptFlingy = depot;
        depot->playIscriptAnim(IscriptAnimation::Landing);
        *currentIscriptUnit = tempUnit;
        *currentIscriptFlingy = tempFlingy;

        depot->status &= ~(UnitStatus::NoCollide);
        depot->orderSignal &= ~0x10;
        depot->sprite->elevationLevel = 4;
        depot->currentButtonSet = depot->id;
      }
    }
    //Is raised -> lower
    else {
      CUnit* const tempUnit = *currentIscriptUnit;
      CUnit* const tempFlingy = *currentIscriptFlingy;
      *currentIscriptUnit = depot;
      *currentIscriptFlingy = depot;
      depot->playIscriptAnim(IscriptAnimation::LiftOff);
      *currentIscriptUnit = tempUnit;
      *currentIscriptFlingy = tempFlingy;

      depot->currentButtonSet = UnitId::UnusedTerran1;  //Buttonset with Raise button
    }
  }
}

namespace hooks {

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
  if (!scbw::isGamePaused()) { //If the game is not paused
    scbw::setInGameLoopState(true); //Needed for scbw::random() to work
    graphics::resetAllGraphics();
    
    //This block is executed once every game.
    if (*elapsedTimeFrames == 0) {
      //Write your code here
      //scbw::printText(PLUGIN_NAME ": Hello, world!");
    }

    //Loop through all visible units in the game.
    for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->link.next) {
      //Write your code here

      //Auto-Repair
      if (unit->id == UnitId::scv && unit->mainOrderId == OrderId::PlayerGuard)
        scvAutoRepair(unit);

      //Lower & raise Supply Depots
      if (unit->id == UnitId::supply_depot && unit->status & UnitStatus::Completed)
        manageSupplyDepot(unit);
    }

    for (int i = 0; i < *clientSelectionCount; ++i) {
      const CUnit *selUnit = clientSelectionGroup->unit[i];

      //Draw attack radius circles for Siege Mode Tanks in current selection
      if (selUnit->id == UnitId::siege_tank_s) {
        graphics::drawCircle(selUnit->getX(), selUnit->getY(),
          selUnit->getMaxWeaponRange(units_dat::GroundWeapon[selUnit->subunit->id]) + 30,
          graphics::TEAL, graphics::ON_MAP);
      }

      //Display rally points for factories selected
      if (selUnit->status & UnitStatus::GroundedBuilding
          && units_dat::GroupFlags[selUnit->id].isFactory
          && (selUnit->playerId == *LOCAL_NATION_ID || scbw::isInReplay())) //Show only if unit is your own or the game is in replay mode
      {
        const CUnit *rallyUnit = selUnit->rally.unit;
        //Rallied to self; disable rally altogether
        if (rallyUnit != selUnit) {
          //Is usable rally unit
          if (rallyUnit && rallyUnit->playerId == selUnit->playerId) {
            graphics::drawLine(selUnit->getX(), selUnit->getY(),
              rallyUnit->getX(), rallyUnit->getY(),
              graphics::GREEN, graphics::ON_MAP);
            graphics::drawCircle(rallyUnit->getX(), rallyUnit->getY(), 10,
              graphics::GREEN, graphics::ON_MAP);
          }
          //Use map position instead
          else if (selUnit->rally.pt.x != 0) {
            graphics::drawLine(selUnit->getX(), selUnit->getY(),
              selUnit->rally.pt.x, selUnit->rally.pt.y,
              graphics::YELLOW, graphics::ON_MAP);
            graphics::drawCircle(selUnit->rally.pt.x, selUnit->rally.pt.y, 10,
              graphics::YELLOW, graphics::ON_MAP);
          }
        }

        //Show worker rally point
        const CUnit *workerRallyUnit = selUnit->moveTarget.unit;
        if (workerRallyUnit) {
          graphics::drawLine(selUnit->getX(), selUnit->getY(),
            workerRallyUnit->getX(), workerRallyUnit->getY(),
            graphics::ORANGE, graphics::ON_MAP);
          graphics::drawCircle(
            workerRallyUnit->getX(), workerRallyUnit->getY(), 10,
            graphics::ORANGE, graphics::ON_MAP);
        }
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

    scbw::setInGameLoopState(false);
  }
  return true;
}

bool gameOn() {
  return true;
}

bool gameEnd() {
  return true;
}

} //hooks
