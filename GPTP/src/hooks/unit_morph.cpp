#include "unit_morph.h"
#include <SCBW/scbwdata.h>
#include <SCBW/enumerations/TechId.h>
#include <SCBW/api.h>
#include <cassert>

namespace hooks {

//Check if @p unit can morph into @p morphUnitId.
bool unitCanMorphHook(const CUnit *unit, u16 morphUnitId) {
  //Default StarCraft behavior

  if (unit->id == UnitId::hydralisk) {
    if (morphUnitId == UnitId::lurker
        && unit->canUseTech(TechId::LurkerAspect, *ACTIVE_NATION_ID) == 1) {
      return true;
    }
  }

  if (unit->id == UnitId::larva || unit->id == UnitId::mutalisk) {
    if (unit->canBuild(morphUnitId, *ACTIVE_NATION_ID) == 1) {
      return true;
    }
  }

  return false;
}

//Check if @p unitId is an egg unit.
bool isEggUnitHook(u16 unitId) {
  //Default StarCraft behavior

  if (unitId == UnitId::egg
      || unitId == UnitId::cocoon
      || unitId == UnitId::ZergLurkerEgg)
    return true;

  return false;
}

//Return the ID of the egg unit to use when morphing @p unitId.
//If the unit cannot morph, return UnitId::None.
u16 getUnitMorphEggTypeHook(u16 unitId) {
  //Default StarCraft behavior

  if (unitId == UnitId::larva)
    return UnitId::egg;

  if (unitId == UnitId::mutalisk)
    return UnitId::cocoon;

  if (unitId == UnitId::hydralisk)
    return UnitId::ZergLurkerEgg;

  return UnitId::None;
}

//Return the ID of unit to revert to when cancelling a morphing @p egg unit.
u16 getCancelUnitChangeTypeHook(const CUnit *unit) {
  //Default StarCraft behavior

  if (unit->id == UnitId::cocoon)
    return UnitId::mutalisk;

  if (unit->id == UnitId::ZergLurkerEgg)
    return UnitId::hydralisk;

  return UnitId::None;  //Default (no revert for larvae)
}

//Check if @p playerId has enough supplies to build @p unitId.
bool hasSuppliesForUnitHook(s8 playerId, u16 unitId, bool canShowErrorMessage) {
  //Default StarCraft behavior
  u32 supplyCost = Unit::SupplyRequired[unitId];

  if (Unit::BaseProperty[unitId] & UnitProperty::TwoUnitsIn1Egg)
    supplyCost *= 2;

  if (unitId == UnitId::lurker)
    supplyCost -= Unit::SupplyRequired[UnitId::hydralisk];

  aiSupplyReserved[playerId] = supplyCost;

  //No supply cost check needed
  if (supplyCost == 0 || Unit::BaseProperty[unitId] & UnitProperty::MorphFromOtherUnit)
    return true;

  const u8 raceId = scbw::getRaceId(unitId);
  assert(raceId <= 2);
  const u32 supplyUsed = raceSupply[raceId].used[playerId];

  //Must construct additional pylons
  if (supplyUsed + supplyCost > raceSupply[raceId].max[playerId]) {
    if (canShowErrorMessage)
      scbw::showErrorMessageWithSfx(playerId, 847 + raceId, 1 + raceId);
    return false;
  }

  //Supply limit exceeded
  if (supplyCost > scbw::getSupplyAvailable(playerId, raceId)) {
    if (canShowErrorMessage)
      scbw::showErrorMessageWithSfx(playerId, 844 + raceId, 153 + raceId);
    return false;
  }

  return true;
}

} //hooks
