#include "cloak_nearby_units.h"
#include "weapon_range.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/unit_finder.h"
#include "../SCBW/enumerations.h"
#include <algorithm>

namespace hooks {

//Helper functions
void refreshSomething();
void secondaryOrder_Cloak(CUnit *unit);


//Cloak all units near @p unit.
void cloakNearbyUnitsHook(CUnit *unit) {
  //Default StarCraft behavior

  //Use the unit's air weapon range
  const unsigned int cloakRadius = getMaxWeaponRange(unit, Unit::AirWeapon[unit->id]);

  //Run the unit finder
  int left    = unit->getX() - cloakRadius;
  int top     = unit->getY() - cloakRadius;
  int right   = unit->getX() + cloakRadius;
  int bottom  = unit->getY() + cloakRadius;
  scbw::UnitFinderResult unitsFound;
  scbw::findUnitsInBounds(left, top, right, bottom, unitsFound);

  bool canRefreshSomething = false;

  for (int i = 0; i < unitsFound.count; ++i) {
    CUnit *foundUnit = unitsFound.units[i];
    //Don't cloak fellow Arbiters and Nukes
    if (foundUnit->id == UnitId::arbiter
        || foundUnit->id == UnitId::Hero_Danimoth
        || foundUnit->id == UnitId::nuclear_missile)
      continue;
    //Don't cloak buildings and neutral accessories (?)
    if (Unit::BaseProperty[foundUnit->id] & (UnitProperty::Building | UnitProperty::NeutralAccessories))
      continue;
    //Hallucinations are not affected
    if (foundUnit->status & UnitStatus::IsHallucination)
      continue;
    //Not sure. Perhaps to prevent warping-in units and buildings from being cloaked?
    if (foundUnit->mainOrderId == OrderId::Warpin)
      continue;
    
    if (foundUnit->playerId == unit->playerId                   //Only cloak units owned by the same player
        && unit->getDistanceToTarget(foundUnit) <= cloakRadius  //Distance check
        ) {
      secondaryOrder_Cloak(foundUnit);
      if (!(foundUnit->status & UnitStatus::CloakingForFree)) {
        foundUnit->status |= UnitStatus::CloakingForFree;
        canRefreshSomething = true;
      }
    }
  }

  //Something related to refreshing graphics
  if (canRefreshSomething)
    refreshSomething();
}


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
    refreshSomething();
  }
}

void refreshSomething() {
  u32*  const bCanUpdateCurrentButtonSet      = (u32*)  0x0068C1B0;
  u8*   const bCanUpdateSelectedUnitPortrait  = (u8*)   0x0068AC74;
  u8*   const unknown1                        = (u8*)   0x0068C1F8;
  u32*  const someDialogUnknown               = (u32*)  0x0068C1E8;
  u32*  const unknown2                        = (u32*)  0x0068C1EC;

  *bCanUpdateCurrentButtonSet = 1;
  *bCanUpdateSelectedUnitPortrait = 1;
  *unknown1 = 1;
  *someDialogUnknown = 0;
  *unknown2 = 0;
}

} //hooks
