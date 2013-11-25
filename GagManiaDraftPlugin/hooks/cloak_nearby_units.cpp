#include "cloak_nearby_units.h"
#include "weapon_range.h"
#include "../SCBW/UnitFinder.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include <algorithm>

//Helper functions
void refreshSomething();
void secondaryOrder_Cloak(CUnit *unit);

namespace hooks {

//Cloak all units near @p cloaker.
void cloakNearbyUnitsHook(CUnit *cloaker) {

  //사거리는 5로 고정
  const unsigned int cloakRadius = 160; //cloaker->getMaxWeaponRange(Unit::AirWeapon[cloaker->id]);

  //Run the unit finder
  int left    = cloaker->getX() - cloakRadius;
  int top     = cloaker->getY() - cloakRadius;
  int right   = cloaker->getX() + cloakRadius;
  int bottom  = cloaker->getY() + cloakRadius;

  scbw::UnitFinder unitsToCloak(left, top, right, bottom);

  bool needsButtonRefresh = false;

  for (int i = 0; i < unitsToCloak.getUnitCount(); ++i) {
    CUnit *unit = unitsToCloak.getUnit(i);

    //서로 클로킹 하지 않음
    if (unit->secondaryOrderId == OrderId::CloakNearbyUnits) 
      continue;

    //핵미사일은 제외
    if (unit->id == UnitId::nuclear_missile)
      continue;

    //동력 끊긴 유닛은 제외
    if (unit->status & UnitStatus::DoodadStatesThing)
      continue;

    //사이오닉 망 동력을 공급하는 유닛은 제외
    if (unit->building.pylonAura)
      continue;

    //Don't cloak buildings and neutral accessories (?)
    if (Unit::BaseProperty[unit->id] & UnitProperty::NeutralAccessories)
      continue;
    //Hallucinations are not affected
    if (unit->status & UnitStatus::IsHallucination)
      continue;
    //Not sure. Perhaps to prevent warping-in units and buildings from being cloaked?
    if (unit->mainOrderId == OrderId::Warpin)
      continue;
    
    //서로 동맹인 경우에만 클로킹해줌
    if (!(scbw::isAlliedTo(unit->playerId, cloaker->playerId)
          && scbw::isAlliedTo(cloaker->playerId, unit->playerId)))
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
