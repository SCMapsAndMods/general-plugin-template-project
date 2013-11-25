#include "psi_field.h"
#include "../SCBW/scbwdata.h"

namespace hooks {

//Check if the given unit id can generate psi fields
bool canMakePsiField(u16 unitId) {
  //Default StarCraft behavior
  if (unitId == UnitId::pylon)
    return true;
  if (unitId == UnitId::shuttle)
	  return true;

  return false;
}

//Actual state check whether a unit can generate a psi field
bool isReadyToMakePsiField(CUnit *unit) {
  //Default StarCraft behavior

  if (unit->id == UnitId::pylon)
    return true;
  if (unit->id == UnitId::shuttle) { 
    for (int i = 0; i < Unit::SpaceProvided[unit->id]; ++i) { 
      CUnit *loadedUnit = unit->getLoadedUnit(i);
      if (loadedUnit && loadedUnit->id == UnitId::probe)
        return true;
    }
  }

  return false;
}

} //hooks
