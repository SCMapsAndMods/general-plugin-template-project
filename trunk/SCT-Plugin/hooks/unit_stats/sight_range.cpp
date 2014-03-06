#include "sight_range.h"
#include <SCBW/enumerations.h>
#include <SCBW/scbwdata.h>
#include <SCBW/api.h>
#include <algorithm>

namespace hooks {

/// Returns the modified sight range of the unit, measured in matrices.
/// StarCraft passes 1 for isForSpellCasting when using Feedback, Mind Control,
/// and Hallucination (but not when launching Nukes).
/// Note: sight ranges cannot exceed 11, unless extended.
u32 getSightRangeHook(const CUnit *unit, bool isForSpellCasting) {
  using scbw::getUpgradeLevel;

  //Check if the unit is a constructing building (exclude remorphing buildings)
  if (unit->status & UnitStatus::GroundedBuilding
      && !(unit->status & UnitStatus::Completed)
      && !unit->isRemorphingBuilding())
    return 4;

  //Upgrades and Ocular Implants do not affect spellcasting range
  if (isForSpellCasting)
    return units_dat::SightRange[unit->id];

  u32 sightRangeBonus = 0;

  //Ocular Implants provides a +2 sight range boost
  if (unit->isBlind)
    sightRangeBonus += 2;

  //Sight range upgrades
  switch (unit->id) {
    case UnitId::spider_mine:
      if (getUpgradeLevel(unit->playerId, UPGRADE_THERMAL_SENSORS))
        sightRangeBonus += 2;
      break;

    //Antennae: Apply to all Zerg air units
    case UnitId::overlord:
    case UnitId::scourge:
    case UnitId::mutalisk:
    case UnitId::queen:
    case UnitId::guardian:
    case UnitId::devourer:
      if (getUpgradeLevel(unit->playerId, UpgradeId::Antennae))
        sightRangeBonus += 2;
      break;

    //Sensor Array: Apply to all Robotics Facility units (excluding Scarabs)
    case UnitId::observer:
    case UnitId::reaver:
    case UnitId::shuttle:
      if (getUpgradeLevel(unit->playerId, UpgradeId::SensorArray))
        sightRangeBonus += 2;
      break;

    //Prescience: Apply to all Gateway units (including Archons and Dark Archons)
    case UnitId::zealot:
    case UnitId::dragoon:
    case UnitId::high_templar:
    case UnitId::dark_templar:
    case UnitId::archon:
    case UnitId::dark_archon:
      if (getUpgradeLevel(unit->playerId, UPGRADE_PRESCIENCE))
        sightRangeBonus += 2;
      break;
  }

  //Hard cap
  return std::min(11u, units_dat::SightRange[unit->id] + sightRangeBonus);
}

} //hooks
