//Contains hooks that control attack range and seek range (AKA target acquisition range).

#include "weapon_range.h"
#include <SCBW/scbwdata.h>
#include <SCBW/enumerations.h>
#include <SCBW/api.h>

namespace hooks {

/// Returns the modified seek range (AKA target acquisition range) for the unit.
/// Note: Seek ranges are measured in matrices (1 matrix = 32 pixels).
/// This hook affects the behavior of CUnit::getSeekRange().
u8 getSeekRangeHook(const CUnit *unit) {
  using UnitStatus::Cloaked;
  using UnitStatus::RequiresDetection;
  using scbw::getUpgradeLevel;

  const u16 unitId = unit->id;

  //Cloaked ghosts do not voluntarily attack enemy units
  if ((unitId == UnitId::ghost
       || unitId == UnitId::sarah_kerrigan
       || unitId == UnitId::Hero_AlexeiStukov
       || unitId == UnitId::Hero_SamirDuran
       || unitId == UnitId::Hero_InfestedDuran)
      && unit->status & (Cloaked | RequiresDetection)
      && unit->mainOrderId != OrderId::HoldPosition2)
    return 0;

  //Arbiter seek range is unaffected by upgrades / Ocular Implants
  if (unitId == UnitId::arbiter || unitId == UnitId::danimoth)
    return Unit::SeekRange[unitId];

  u8 bonusAmount = 0;
  switch (unitId) {
    case UnitId::marine:
    case UNIT_FIEND:
      if (getUpgradeLevel(unit->playerId, UpgradeId::U_238Shells))
        bonusAmount = 1;
      break;
    case UnitId::hydralisk:
      if (getUpgradeLevel(unit->playerId, UpgradeId::GroovedSpines))
        bonusAmount = 1;
      break;
    case UnitId::dragoon:
      if (getUpgradeLevel(unit->playerId, UpgradeId::SingularityCharge))
        bonusAmount = 2;
      break;
    case UnitId::fenix_dragoon:
      if (scbw::isBroodWarMode())
        bonusAmount = 2;
      break;
    case UnitId::goliath:
    case UnitId::goliath_turret:
      if (getUpgradeLevel(unit->playerId, UpgradeId::CharonBooster))
        bonusAmount = 3;
      break;
    case UnitId::alan_schezar:
    case UnitId::alan_schezar_turret:
      if (scbw::isBroodWarMode())
        bonusAmount = 3;
      break;
    
    //Added
    case UnitId::spider_mine:
      if (getUpgradeLevel(unit->playerId, UPGRADE_THERMAL_SENSORS))
        bonusAmount = 1;
      break;
  }

  //Added: Ocular Implants adds a flat +1 seek range
  if (unit->isBlind || unit->subunit && unit->subunit->isBlind)
    bonusAmount += 1;

  return Unit::SeekRange[unitId] + bonusAmount;
}

/// Returns the modified max range for the weapon, which is assumed to be
/// attached to the given unit.
/// This hook affects the behavior of CUnit::getMaxWeaponRange().
/// Note: Weapon ranges are measured in pixels.
///
/// @param  weapon    The weapons.dat ID of the weapon.
/// @param  unit      The unit that owns the weapon. Use this to check upgrades.
u32 getMaxWeaponRangeHook(const CUnit *unit, u8 weaponId) {
  using scbw::getUpgradeLevel;

  //Arbiter weapon range is unaffected, because:
  // 1. Hooks for splash radius modifiers have not been implemented
  // 2. Increased splash radius would be OP
  // 3. It would also increase cloaking radius (also OP)
  if (unit->id == UnitId::arbiter || unit->id == UnitId::danimoth)
    if (weaponId == WEAPON_MEDIATION_FIELD
        || weaponId == WEAPON_MEDIATION_FIELD_HERO)
      return weapons_dat::MaxRange[weaponId];

  u32 bonusAmount = 0;

  //Give bonus range to units inside Bunkers
  if (unit->status & UnitStatus::InBuilding)
    bonusAmount = 64;

  switch (unit->id) {
    case UnitId::marine:
    case UNIT_FIEND:
      if (getUpgradeLevel(unit->playerId, UpgradeId::U_238Shells))
        bonusAmount += 32;
      break;
    case UnitId::hydralisk:
      if (getUpgradeLevel(unit->playerId, UpgradeId::GroovedSpines))
        bonusAmount += 32;
      break;
    case UnitId::dragoon:
      if (getUpgradeLevel(unit->playerId, UpgradeId::SingularityCharge))
        bonusAmount += 64;
      break;
    case UnitId::fenix_dragoon:
      if (scbw::isBroodWarMode())
        bonusAmount += 64;
      break;
    case UnitId::goliath:
    case UnitId::goliath_turret:
      if (weaponId == WeaponId::HellfireMissilePack
          && getUpgradeLevel(unit->playerId, UpgradeId::CharonBooster))
        bonusAmount += 96;
      break;
    case UnitId::alan_schezar:
    case UnitId::alan_schezar_turret:
      if (weaponId == WeaponId::HellfireMissilePack_AlanSchezar
          && scbw::isBroodWarMode())
        bonusAmount += 96;
      break;
  }

  //Added: Ocular Implants adds a flat +32 weapon range (to ranged units only)
  // Spellcasting range is not affected at all
  if (unit->isBlind || unit->subunit && unit->subunit->isBlind) {
    // Check weapon ID to see if it's a spell
    if (weaponId != WeaponId::Lockdown
      && weaponId != WeaponId::Irradiate
      && weaponId != WeaponId::EMP_Shockwave
      && weaponId != WeaponId::YamatoGun
      && weaponId != WEAPON_OCULAR_IMPLANTS
      && weaponId != WeaponId::Restoration
      && weaponId != WeaponId::Ensnare
      && weaponId != WEAPON_CARNIVOROUS_MITES
      && weaponId != WeaponId::DarkSwarm
      && weaponId != WeaponId::Consume
      && weaponId != WeaponId::Plague
      && weaponId != WeaponId::PsiStorm
      && weaponId != WeaponId::Maelstrom
      && weaponId != WEAPON_TELEPATHIC_TRACE
      && weaponId != WeaponId::DisruptionWeb
      && weaponId != WeaponId::StasisField)
    {
      if (weapons_dat::MaxRange[weaponId] >= 64)
        bonusAmount += 32;
    }
  }

  return weapons_dat::MaxRange[weaponId] + bonusAmount;
}

} //hooks
