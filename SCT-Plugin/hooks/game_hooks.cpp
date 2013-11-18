/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../graphics/graphics.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/ExtendSightLimit.h"
#include "psi_field.h"
#include <SCBW/UnitFinder.h>
#include <cstdio>

bool firstRun = true;

//Custom helper class
class RepairTargetFinder: public scbw::UnitFinderCallbackMatchInterface {
  const CUnit *scv;
  public:
    void setScv(const CUnit *scv) { this->scv = scv; }
    bool match(const CUnit *unit) {
      if (!unit)
        return false;
      if (unit->playerId != scv->playerId)
        return false;
      if (unit == scv)
        return false; //Prevent repairing self

      GroupFlag gf = Unit::GroupFlags[unit->id];
      if (!gf.isTerran || gf.isZerg || gf.isProtoss)
        return false;
      
      if (scv->getDistanceToTarget(unit) > 128) //Repair distance
        return false;

      if (unit->hitPoints >= Unit::MaxHitPoints[unit->id])
        return false;
      if (unit->stasisTimer)
        return false;
      if (unit->status & UnitStatus::InTransport)
        return false;
      if (!(Unit::BaseProperty[unit->id] & UnitProperty::Mechanical))
        return false;
      if (!(unit->status & UnitStatus::Completed))
        return false;
      if (!scv->hasPathToUnit(unit))
        return false;

      return true;
    }
};

RepairTargetFinder repairTargetFinder;

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

      //Auto-Repair
      if (unit->id == UnitId::scv && unit->mainOrderId == OrderId::PlayerGuard) {
        repairTargetFinder.setScv(unit);
        const CUnit *repairTarget = scbw::UnitFinder::getNearest(
          unit->getX(), unit->getY(),
          unit->getX() - 200, unit->getY() - 200,
          unit->getX() + 200, unit->getY() + 200,
          repairTargetFinder);

        if (repairTarget)
          unit->orderTo(OrderId::Repair1, repairTarget);
      }

      //Set nexus color
      if (unit->id == UnitId::nexus
          && scbw::getUpgradeLevel(unit->playerId, UPGRADE_PARTICLE_FILTER))
      {
        CImage *nexusAttackOverlay = unit->getOverlay(IMAGE_NEXUS_ATTACK_OVERLAY);
        if (nexusAttackOverlay)
          nexusAttackOverlay->setRemapping(ColorRemapping::BFire);
      }
    }

    for (int i = 0; i < *clientSelectionCount; ++i) {
      const CUnit *selUnit = clientSelectionGroup->unit[i];

      //Draw attack radius circles for Siege Mode Tanks in current selection
      if (selUnit->id == UnitId::siege_tank_s) {
        graphics::drawCircle(selUnit->getX(), selUnit->getY(),
          selUnit->getMaxWeaponRange(Unit::GroundWeapon[selUnit->subunit->id]) + 30,
          graphics::TEAL, graphics::ON_MAP);
      }

      //Display rally points for factories selected
      if (selUnit->status & UnitStatus::GroundedBuilding
          && Unit::GroupFlags[selUnit->id].isFactory
          && selUnit->playerId == *LOCAL_NATION_ID) //Doesn't work if it's not your own building
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
