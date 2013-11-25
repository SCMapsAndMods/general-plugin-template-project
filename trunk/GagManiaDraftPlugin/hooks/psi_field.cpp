#include "psi_field.h"
#include "../SCBW/scbwdata.h"

namespace hooks {

//Check if the given unit id can generate psi fields
bool canMakePsiField(u16 unitId) {
  
  //파일런 + 셔틀
  if (unitId == UnitId::pylon || unitId == UnitId::shuttle)
    return true;
  return false;
}

//Actual state check whether a unit can generate a psi field
bool isReadyToMakePsiField(CUnit *unit) {

  if (unit->id == UnitId::pylon)
    return true;

  //셔틀에 프로브가 타고 있을 경우 사이오닉 망 생성
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
