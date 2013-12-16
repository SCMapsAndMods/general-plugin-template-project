#include "weapon_damage.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include <algorithm>

namespace {
//Helper functions
void createShieldOverlay(CUnit *unit, u32 attackDirection);
u16 getUnitStrength(const CUnit *unit, bool useGroundStrength);

/// Definition of damage factors (explosive, concussive, etc.)
struct {
  s32 damageType;
  s32 unitSizeFactor[4];   //Order: {independent, small, medium, large}
} const damageFactor[5] = {
  {0, 0, 0, 0, 0},        //Independent
  {1, 0, 128, 192, 256},  //Explosive
  {2, 0, 256, 128, 64},   //Concussive
  {3, 0, 256, 256, 256},  //Normal
  {4, 0, 256, 256, 256}   //IgnoreArmor
};

} //unnamed namespace

namespace hooks {

/// Hooks into the CUnit::damageWith() function.
void weaponDamageHook(s32     damage,
                      CUnit*  target,
                      u8      weaponId,
                      CUnit*  attacker,
                      s8      attackingPlayer,
                      s8      direction,
                      u8      dmgDivisor) {

  using scbw::isCheatEnabled;
  using CheatFlags::PowerOverwhelming;

  //Don't bother if the unit is already dead or invincible
  if (target->hitPoints == 0 || (target->status & UnitStatus::Invincible))
    return;

  if (isCheatEnabled(PowerOverwhelming)                           //If Power Overwhelming is enabled
      && playerTable[target->playerId].type == PlayerType::Human) //인간 플레이어의 유닛이 맞을 때만 무적 치트 적용 (인공지능끼리는 무시)
    return;

  if (target->status & UnitStatus::IsHallucination)
    damage *= 2;

  damage = damage / dmgDivisor + (target->acidSporeCount << 8);
  if (damage < 128)
    damage = 128;

  //Reduce Defensive Matrix
  if (target->defensiveMatrixHp) {
    const s32 d_matrix_reduceAmount = std::min<s32>(damage, target->defensiveMatrixHp);
    damage -= d_matrix_reduceAmount;
    target->reduceDefensiveMatrixHp(d_matrix_reduceAmount);
  }

  //파이어뱃 파란불 업글 적용
  if (attacker && attacker->id == UnitId::firebat
      && weaponId == WeaponId::FlameThrower
      && Unit::BaseProperty[target->id] & UnitProperty::Organic
      && scbw::getUpgradeLevel(attacker->playerId, UPGRADE_FIREBAT_BLUE_FLAME))
    damage += 512;  //데미지 +2

  const u8 damageType = Weapon::DamageType[weaponId];

  //Reduce Plasma Shields...but not just yet
  bool isHardenedShieldsActivated = false;
  s32 shieldReduceAmount = 0;
  if (Unit::ShieldsEnabled[target->id] && target->shields >= 256) {
    if (damageType != DamageType::IgnoreArmor) {
      s32 plasmaShieldUpg = scbw::getUpgradeLevel(target->playerId, UpgradeId::ProtossPlasmaShields) << 8;

      //강화 보호막 적용
      if (target->id == UnitId::dragoon && target->stimTimer == 0&&target->shields >= 2560) {
		  if (damage >= 2560){
          damage = 2560;
        isHardenedShieldsActivated = true;
		  }
      }

      if (damage > plasmaShieldUpg) //Weird logic, Blizzard dev must have been sleepy
        damage -= plasmaShieldUpg;
      else
        damage = 128;
    }
    shieldReduceAmount = std::min(damage, target->shields);
    damage -= shieldReduceAmount;
  }

  //Apply armor
  if (damageType != DamageType::IgnoreArmor) {
    const s32 armorTotal = target->getArmor() << 8;
    damage -= std::min(damage, armorTotal);
  }

  //Apply damage type/unit size factor
  damage = (damage * damageFactor[damageType].unitSizeFactor[Unit::SizeType[target->id]]) >> 8;
  if (shieldReduceAmount == 0 && damage < 128)
    damage = 128;

  //Deal damage to target HP, killing it if possible
  target->damageHp(damage, attacker, attackingPlayer,
                   weaponId != WeaponId::Irradiate);    //Prevent Science Vessels from being continuously revealed to the irradiated target

  //Reduce shields (finally)
  if (shieldReduceAmount != 0) {
    if (damageType != DamageType::Independent && target->shields != 0) {
      if (isHardenedShieldsActivated)
        target->sprite->createTopOverlay(378);  //강화 보호막 그래픽 효과 적용
      else
        createShieldOverlay(target, direction);
    }
    
    //순서 바꿈 (실드가 0이 되는 순간에도 그래픽은 보이게)
    target->shields -= shieldReduceAmount;
  }

  //Update unit strength data (?)
  target->airStrength = getUnitStrength(target, false);
  target->groundStrength = getUnitStrength(target, true);
}

} //hooks

namespace {

/**** Definitions of helper functions. Do NOT modify anything below! ****/

//Creates the Plasma Shield flickering effect.
//Identical to function @ 0x004E6140
void createShieldOverlay(CUnit *unit, u32 attackDirection) {
  const LO_Header* shield_lo = shieldOverlays[unit->sprite->mainGraphic->id];
  u32 frameAngle = (attackDirection - 124) / 8 % 32;
  Point8 offset = shield_lo->getOffset(unit->sprite->mainGraphic->direction, frameAngle);
  unit->sprite->createOverlay(ImageId::ShieldOverlay, offset.x, offset.y, frameAngle);
}

//Somehow related to AI stuff; details unknown.
const u32 Helper_GetUnitStrength      = 0x00431800;
u16 getUnitStrength(const CUnit *unit, bool useGroundStrength) {
  u16 strength;
  u32 useGroundStrength_ = (useGroundStrength ? 1 : 0);

  __asm {
    PUSHAD
    PUSH useGroundStrength_
    MOV EAX, unit
    CALL Helper_GetUnitStrength
    MOV strength, AX
    POPAD
  }

  return strength;
}

} //unnamed namespace
