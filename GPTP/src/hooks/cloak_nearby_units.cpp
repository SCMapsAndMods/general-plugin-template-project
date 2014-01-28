#include "cloak_nearby_units.h"
#include <SCBW/UnitFinder.h>
#include <SCBW/enumerations.h>
#include <SCBW/api.h>
#include <algorithm>

//Helper functions
void refreshSomething();
void secondaryOrder_Cloak(CUnit *unit);

namespace hooks {

//Cloak all units near @p cloaker.
void cloakNearbyUnitsHook(CUnit *cloaker) {
  //Default StarCraft behavior

  //Use the unit's air weapon range
  const unsigned int cloakRadius = cloaker->getMaxWeaponRange(units_dat::AirWeapon[cloaker->id]);

  //Run the unit finder
  int left    = cloaker->getX() - cloakRadius;
  int top     = cloaker->getY() - cloakRadius;
  int right   = cloaker->getX() + cloakRadius;
  int bottom  = cloaker->getY() + cloakRadius;

  scbw::UnitFinder unitsToCloak(left, top, right, bottom);

  bool needsButtonRefresh = false;

  for (int i = 0; i < unitsToCloak.getUnitCount(); ++i) {
    CUnit *unit = unitsToCloak.getUnit(i);
    //Don't cloak fellow Arbiters and Nukes
    if (unit->id == UnitId::arbiter
        || unit->id == UnitId::Hero_Danimoth
        || unit->id == UnitId::nuclear_missile)
      continue;
    //Don't cloak buildings and neutral accessories (?)
    if (units_dat::BaseProperty[unit->id] & (UnitProperty::Building | UnitProperty::NeutralAccessories))
      continue;
    //Hallucinations are not affected
    if (unit->status & UnitStatus::IsHallucination)
      continue;
    //Not sure. Perhaps to prevent warping-in units and buildings from being cloaked?
    if (unit->mainOrderId == OrderId::Warpin)
      continue;

    //Only cloak units owned by the same player
    if (cloaker->playerId != unit->playerId)
      continue;
    
    //Distance check
    if (cloaker->getDistanceToTarget(unit) <= cloakRadius) {
      secondaryOrder_Cloak(unit);
      //Remove energy cost for units that use energy to cloak
      if (!(unit->status & UnitStatus::CloakingForFree)) {
        unit->status |= UnitStatus::CloakingForFree;
        needsButtonRefresh = true;
      }
    }
  }

  if (needsButtonRefresh)
    scbw::refreshConsole();
}

} //hooks

/**** Helper function definitions. Do not change anything below this! ****/

void secondaryOrder_Cloak(CUnit *unit) {
  CUnit** const firstBurrowedUnit = (CUnit**) 0x0063FF5C;

  if (unit->isCloaked++)
    return;

  //If the unit has not been added to the cloaked/burrowed linked list yet
  if (!(unit->status & UnitStatus::RequiresDetection)
      && unit->nextBurrowedUnit == NULL
      ) {
    unit->nextBurrowedUnit      = *firstBurrowedUnit;
    unit->previousBurrowedUnit  = NULL;
    if (*firstBurrowedUnit != NULL)
      (*firstBurrowedUnit)->previousBurrowedUnit = unit;
    *firstBurrowedUnit = unit;

    scbw::refreshConsole();
  }
}
