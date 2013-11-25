#include "../SCBW/api.h"
#include "../SCBW/enumerations/WeaponId.h"
#include "weapon_range.h"
#include <cstdio>

//-------- Helper functions --------//

char buffer[128];

u8 getDamageFactorForTooltip(u8 weaponId, const CUnit *unit) {
  //Default StarCraft behavior
  if (unit->id == UnitId::firebat || unit->id == UnitId::gui_montag
      || unit->id == UnitId::zealot || unit->id == UnitId::fenix_zealot)
    return 2;

  if (unit->id == UnitId::valkyrie)
    return 1;

  return Weapon::DamageFactor[weaponId];
}

u16 getBaseWeaponDamageDisplayed(u8 weaponId, const CUnit *unit) {
  //Default StarCraft behavior
  const u8 damageFactor = getDamageFactorForTooltip(weaponId, unit);
  return Weapon::DamageAmount[weaponId] * damageFactor;
}

u16 getBonusWeaponDamageDisplayed(u8 weaponId, const CUnit *unit) {
  //Default StarCraft behavior
  const u8 damageFactor = getDamageFactorForTooltip(weaponId, unit);
  const u8 upgradeLevel = scbw::getUpgradeLevel(unit->playerId, Weapon::DamageUpgrade[weaponId]);
  return Weapon::DamageBonus[weaponId] * damageFactor * upgradeLevel;
}

//Precondition: @p entryStrIndex is a stat_txt.tbl string index.
const char* getWeaponTooltipString(u8 weaponId, const CUnit *unit, u16 entryStrIndex) {
  //Default StarCraft behavior

  const char *entryName = scbw::getStatTxtTblString(entryStrIndex);
  const char *damageStr = scbw::getStatTxtTblString(777);         //"Damage:"

  const u8 damageFactor = getDamageFactorForTooltip(weaponId, unit);

  const u16 baseDamage = getBaseWeaponDamageDisplayed(weaponId, unit);
  const u16 bonusDamage = getBonusWeaponDamageDisplayed(weaponId, unit);

  const u32 WeaponRange = hooks::getMaxWeaponRangeHook(unit,weaponId);
  const u32 PlusRange = Weapon::MaxRange[weaponId];

  const char *Typecheck[5];
  Typecheck[0]="Independent";
  Typecheck[1]="폭팔형";
  Typecheck[2]="진동형";
  Typecheck[3]="일반";
  Typecheck[4]="방어력무시형";


  if (weaponId == WeaponId::HaloRockets) {
    if (bonusDamage > 0) {
      const char *perRocketStr = scbw::getStatTxtTblString(1301); //"per rocket"
	  sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d\n\n%s %d+%d %s",
		  entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32, damageStr, baseDamage, bonusDamage, perRocketStr);
    }

	else {
		const char *perRocketStr = scbw::getStatTxtTblString(1301); //"per rocket"
      sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d\n\n%s %d %s",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32, damageStr, baseDamage, perRocketStr);
	}
	  
  }
  else if (unit->id==UnitId::firebat&&weaponId == WeaponId::FlameThrower&&scbw::getUpgradeLevel(unit->playerId,UpgradeId::UnusedUpgrade59)) {
    if (bonusDamage > 0)
		sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d\n\n%s %d+%d\x11(+4)\x4",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32, damageStr, baseDamage, bonusDamage);
    else
      sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d\n\n%s %d\x11(+4)\x4",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32, damageStr, baseDamage);
	   
    }
  else {
	  if(PlusRange!=WeaponRange){
if (bonusDamage > 0)
      sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d+%d\n\n%s %d+%d",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32,(WeaponRange-PlusRange)/32, damageStr, baseDamage, bonusDamage);
    else
      sprintf_s(buffer, sizeof(buffer), "%s\n\n유형: %s\n\n사거리: %d+%d\n\n%s %d",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32,(WeaponRange-PlusRange)/32, damageStr, baseDamage);
		   
	  }
	  
	  else {
		  
    if (bonusDamage > 0)
      sprintf_s(buffer, sizeof(buffer), "%s\n\n유형:%s\n\n사거리:%d\n\n%s %d+%d",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32, damageStr, baseDamage, bonusDamage);
    else
      sprintf_s(buffer, sizeof(buffer), "%s\n\n유형:%s\n\n사거리:%d\n\n%s %d",
                entryName, Typecheck[Weapon::DamageType[weaponId]],WeaponRange/32, damageStr, baseDamage);
	  }
  }


  return buffer;
}

//-------- Actual hook functions --------//

namespace hooks {

const char* getWeaponTooltipString(u8 weaponId, const CUnit *unit) {
  return getWeaponTooltipString(weaponId, unit, Weapon::Label[weaponId]);
}

const char* getArmorTooltipString(const CUnit *unit) {
  //Default StarCraft behavior

  const char *armorUpgradeName = scbw::getStatTxtTblString(Upgrade::Label[Unit::ArmorUpgrade[unit->id]]);
  const char *armorStr = scbw::getStatTxtTblString(778);          //"Armor:"
  const char *sizecheck[4];
  sizecheck[0]="Independent";
  sizecheck[1]="소형";
  sizecheck[2]="중형";
  sizecheck[3]="대형";

  const u8 baseArmor = Unit::ArmorAmount[unit->id];
  const u8 bonusArmor = unit->getArmorBonus();

  if (bonusArmor > 0)
	  sprintf_s(buffer, sizeof(buffer), "%s\n\n크기: %s\n\n%s %d+%d",
	  armorUpgradeName, sizecheck[Unit::SizeType[unit->id]], armorStr, baseArmor, bonusArmor);
  else
    sprintf_s(buffer, sizeof(buffer), "%s\n\n크기: %s\n\n%s %d",
              armorUpgradeName, sizecheck[Unit::SizeType[unit->id]], armorStr, baseArmor);

  return buffer;
}

const char* getShieldTooltipString(const CUnit *unit) {
  //Default StarCraft behavior

  const char *shieldUpgradeName = scbw::getStatTxtTblString(Upgrade::Label[UpgradeId::ProtossPlasmaShields]);
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

const char* getSpecialTooltipString(u16 sourceUnitId, const CUnit *unit) {
  //Default StarCraft behavior

  if (sourceUnitId == UnitId::interceptor) {
    return getWeaponTooltipString(WeaponId::PulseCannon, unit, 791);  //"Interceptors"
  }

  if (sourceUnitId == UnitId::ProtossScarab) {
    return getWeaponTooltipString(WeaponId::Scarab, unit, 792);       //"Scarabs"
  }

  if (sourceUnitId == UnitId::nuclear_missile) {
    return scbw::getStatTxtTblString(793);  //"Nukes"
  }

  if (sourceUnitId == UnitId::spider_mine) {
    return getWeaponTooltipString(WeaponId::SpiderMines, unit, 794);  //"Spider Mines"
  }

  //Should never reach here
  return "";
}

} //hooks

