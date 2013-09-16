#include "psi_field.h"
#include "../SCBW/scbwdata.h"

namespace hooks {

//Unit type check (used in unit destructor)
bool canMakePsiField(u16 unitId) {
  if (unitId == UnitId::pylon || unitId == UnitId::shuttle)
    return true;
  return false;
}

//Actual check whether a unit can generate a psi field
bool isReadyToMakePsiField(CUnit *unit) {
  //NOT default StarCraft behavior

  if (unit->id == UnitId::pylon)
    return true;

  if (unit->id == UnitId::shuttle) {
    for (int i = 0; i < 8; ++i) {
      if (unit->loadedUnitIndex[i] == 0) continue;
      const int unitIndex = unit->loadedUnitIndex[i] % 2048 - 1;
      const CUnit *loadedUnit = &unitTable[unitIndex];

      if (loadedUnit->mainOrderId != OrderId::Die
          && loadedUnit->id == UnitId::probe) {
        return true;
      }
    }
  }

  return false;
}

} //hooks
