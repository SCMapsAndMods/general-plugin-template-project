#include "unit_armor_bonus.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

/// Returns the bonus armor for this unit.
u8 getArmorBonusHook(const CUnit *unit) {
  //Default StarCraft behavior
  using scbw::getUpgradeLevel;

  u8 armorUpg = 0;
  if (scbw::isBroodWarMode()) {
    if (unit->id == UnitId::Hero_Torrasque || unit->id == UnitId::ultralisk) {
      if ((Unit::BaseProperty[unit->id] & UnitProperty::Hero)
          || getUpgradeLevel(unit->playerId, UpgradeId::ChitinousPlating)) {
        armorUpg = 2;
      }
    }
  }
  return armorUpg + getUpgradeLevel(unit->playerId, Unit::ArmorUpgrade[unit->id]);
}
