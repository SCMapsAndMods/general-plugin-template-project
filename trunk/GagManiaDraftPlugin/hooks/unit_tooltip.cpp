#include "../SCBW/api.h"
#include "../SCBW/enumerations/WeaponId.h"
#include <cstdio>


char buffer[128];

//Returns the special damage multiplier factor for units that don't use the
//"Damage Factor" property in weapons.dat.
u8 getDamageFactorForTooltip(u8 weaponId, const CUnit *unit) {
  //Default StarCraft behavior
  if (unit->id == UnitId::firebat || unit->id == UnitId::gui_montag
      || unit->id == UnitId::zealot || unit->id == UnitId::fenix_zealot)
    return 2;

  if (unit->id == UnitId::valkyrie)
    return 1;

  return Weapon::DamageFactor[weaponId];
}

//Returns the C-string for the tooltip text of the unit's weapon icon.
//This function is used for weapon icons and special icons.
//Precondition: @p entryStrIndex is a stat_txt.tbl string index.
const char* getWeaponTooltipString(u8 weaponId, const CUnit *unit, u16 entryStrIndex) {

  const char *entryName = scbw::getStatTxtTblString(entryStrIndex);
  const char *damageStr = scbw::getStatTxtTblString(777);         //"Damage:"

  const u8 damageFactor = getDamageFactorForTooltip(weaponId, unit);
  const u8 upgradeLevel = scbw::getUpgradeLevel(unit->playerId, Weapon::DamageUpgrade[weaponId]);
  const u16 baseDamage = Weapon::DamageAmount[weaponId] * damageFactor;
  const u16 bonusDamage = Weapon::DamageBonus[weaponId] * damageFactor * upgradeLevel;

  //공격력 업그레이드
  char bonusDamageStr[10] = "";
  if (bonusDamage > 0)
    sprintf_s(bonusDamageStr, sizeof(bonusDamageStr), "+%d", bonusDamage);

  //끝에 붙는 문자열 (발키리 + 파이어뱃)
  char appendStr[20] = "";
  if (weaponId == WeaponId::HaloRockets)
    sprintf_s(appendStr, sizeof(appendStr), " %s", scbw::getStatTxtTblString(1301));  //"per rocket"
  else if (weaponId == WeaponId::FlameThrower
           && scbw::getUpgradeLevel(unit->playerId, UPGRADE_FIREBAT_BLUE_FLAME))
    sprintf_s(appendStr, sizeof(appendStr), "\x11(+4)\x4");

  //공격력 유형
  const char *damageTypeStr = "";
  switch (Weapon::DamageType[weaponId]) {
    case 0: damageTypeStr = "Independent"; break;
    case 1: damageTypeStr = "폭발형"; break;
    case 2: damageTypeStr = "진동형"; break;
    case 3: damageTypeStr = "일반"; break;
    case 4: damageTypeStr = "방어력 무시"; break;
  }

  //사거리
  const u32 currentWeaponRange = unit->getMaxWeaponRange(weaponId);
  const u32 baseWeaponRange = Weapon::MaxRange[weaponId];
  char weaponRangeStr[10];

  if (currentWeaponRange != baseWeaponRange)
    sprintf_s(weaponRangeStr, sizeof(weaponRangeStr), "%d+%d",
              baseWeaponRange, currentWeaponRange - baseWeaponRange);
  else
    sprintf_s(weaponRangeStr, sizeof(weaponRangeStr), "%d", baseWeaponRange);

  sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d\n\n%s %d%s%s",
            entryName, damageTypeStr, weaponRangeStr, damageStr, baseDamage, bonusDamageStr, appendStr);

  return buffer;
}

namespace hooks {

//Returns the C-string for the tooltip text of the unit's weapon icon.
const char* getWeaponTooltipString(u8 weaponId, const CUnit *unit) {
  return getWeaponTooltipString(weaponId, unit, Weapon::Label[weaponId]);
}

//Returns the C-string for the tooltip text of the unit's armor icon.
const char* getArmorTooltipString(const CUnit *unit) {
  
  const u16 labelId = Upgrade::Label[Unit::ArmorUpgrade[unit->id]];
  const char *armorUpgradeName = scbw::getStatTxtTblString(labelId);
  const char *armorStr = scbw::getStatTxtTblString(778);          //"Armor:"

  const u8 baseArmor = Unit::ArmorAmount[unit->id];
  const u8 bonusArmor = unit->getArmorBonus();

  //방어력 업그레이드
  char bonusArmorStr[10] = "";
  if (bonusArmor > 0)
    sprintf_s(bonusArmorStr, sizeof(bonusArmorStr), "+%d", bonusArmor);

  //유닛 크기
  const char *unitSizeStr = "";
  switch (Unit::SizeType[unit->id]) {
    case 0: unitSizeStr = "Independent"; break;
    case 1: unitSizeStr = "소형"; break;
    case 2: unitSizeStr = "중형"; break;
    case 3: unitSizeStr = "대형"; break;
  }

  sprintf_s(buffer, sizeof(buffer), "%s\n\n크기: %s\n\n%s %d%s",
            armorUpgradeName, unitSizeStr, armorStr, baseArmor, bonusArmorStr);

  return buffer;
}


//Returns the C-string for the tooltip text of the plasma shield icon.
const char* getShieldTooltipString(const CUnit *unit) {
  //Default StarCraft behavior

  const u16 labelId = Upgrade::Label[UpgradeId::ProtossPlasmaShields];
  const char *shieldUpgradeName = scbw::getStatTxtTblString(labelId);
  const char *shieldStr = scbw::getStatTxtTblString(779);         //"Shields:"

  const u8 shieldUpgradeLevel = scbw::getUpgradeLevel(unit->playerId, UpgradeId::ProtossPlasmaShields);

  if (shieldUpgradeLevel > 0)
    sprintf_s(buffer, sizeof(buffer), "%s\n%s %d+%d",
              shieldUpgradeName, shieldStr, 0, shieldUpgradeLevel);
  else
    sprintf_s(buffer, sizeof(buffer), "%s\n%s %d",
              shieldUpgradeName, shieldStr, 0);

  return buffer;
}

//Returns the C-string for the tooltip text of the Interceptor icon (Carriers),
//Scarab icon (Reavers), Nuclear Missile icon (Nuclear Silos), and Spider Mine
//icon (Vultures).
const char* getSpecialTooltipString(u16 iconUnitId, const CUnit *unit) {
  //Default StarCraft behavior

  if (iconUnitId == UnitId::interceptor) {
    return getWeaponTooltipString(WeaponId::PulseCannon, unit, 791);  //"Interceptors"
  }

  if (iconUnitId == UnitId::scarab) {
    return getWeaponTooltipString(WeaponId::Scarab, unit, 792);       //"Scarabs"
  }

  if (iconUnitId == UnitId::nuclear_missile) {
    return scbw::getStatTxtTblString(793);  //"Nukes"
  }

  if (iconUnitId == UnitId::spider_mine) {
    return getWeaponTooltipString(WeaponId::SpiderMines, unit, 794);  //"Spider Mines"
  }

  //Should never reach here
  return "";
}

} //hooks

