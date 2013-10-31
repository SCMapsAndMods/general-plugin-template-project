#include "building_morph.h"
#include <SCBW/enumerations.h>

namespace hooks {

//Checks if @p unitId is a building that can be morphed from another building.
bool isMorphedBuildingHook(u16 unitId) {
  //Default StarCraft behavior
  if (unitId == UnitId::lair
      || unitId == UnitId::hive
      || unitId == UnitId::greater_spire
      || unitId == UnitId::sunken_colony
      || unitId == UnitId::spore_colony)
    return true;

  return false;
}

} //hooks
