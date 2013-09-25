#include <cassert>
#include "CUnit.h"
#include "../api.h"
#include "../enumerations.h"


const u32 Func_SetUnitHp = 0x00467340;
void CUnit::setHp(s32 hitPoints) {
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    MOV ECX, hitPoints
    CALL Func_SetUnitHp
    POPAD
  }
}

extern const u32 Func_DamageWith = 0x00479930; //Note: Also used by doWeaponDamageHook()
void CUnit::damageWith(s32 damage, u8 weaponId, CUnit *attacker,
                       s8 attackingPlayer, s8 direction, u8 damageDivisor) {
  assert(this);
  assert(weaponId < WEAPON_TYPE_COUNT);
  assert(damageDivisor != 0);

  u32 weaponId_         = weaponId;
  s32 attackingPlayer_  = attackingPlayer;
  s32 direction_        = direction;
  s32 damageDivisor_    = damageDivisor;
  __asm {
    PUSHAD
    MOV EDI, this
    MOV EAX, damage
    PUSH attackingPlayer_
    PUSH attacker
    PUSH direction_
    PUSH damageDivisor_
    PUSH weaponId_
    CALL Func_DamageWith
    POPAD    
  }
}

const u32 Func_DamageUnitHp = 0x004797B0;
void CUnit::damageHp(s32 damage, CUnit *attacker, s32 attackingPlayer, bool notify) {
  assert(this);

  u32 notify_ = notify ? 1 : 0;
  __asm {
    PUSHAD
    PUSH notify_
    PUSH attackingPlayer
    PUSH attacker
    MOV EAX, damage
    MOV ECX, this
    CALL Func_DamageUnitHp
    POPAD
  }
}

//Logically equivalent to function @ 0x00454ED0
void CUnit::reduceDefensiveMatrixHp(s32 amount) {
  assert(this);

  if (this->defensiveMatrixHp > amount) {
    this->defensiveMatrixHp -= amount;
  }
  else {
    this->defensiveMatrixHp = 0;
    this->defensiveMatrixTimer = 0;
    this->removeOverlay(ImageId::DefensiveMatrixFront_Small,
                        ImageId::DefensiveMatrixFront_Large);
    this->removeOverlay(ImageId::DefensiveMatrixBack_Small,
                        ImageId::DefensiveMatrixBack_Large);
  }
  if (this->defensiveMatrixTimer && !(this->status & UnitStatus::Burrowed)) {
    CUnit *unit = this->subunit ? this->subunit : this;
    unit->sprite->createTopOverlay(scbw::getUnitOverlayAdjustment(unit) + ImageId::DefensiveMatrixHit_Small);
  }
}

const u32 Func_RemoveUnit = 0x00475710; //AKA orders_SelfDestructing()
void CUnit::remove() {
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    CALL Func_RemoveUnit
    POPAD
  }
}

const u32 Func_OrderToUnit = 0x004752B0; //AKA AssignOrderWithTarget(); Primarily for use with orderNewUnitToRally(), but may have other uses.
void CUnit::orderTo(u32 orderId, const CUnit *target) {
  assert(this);
  assert(target);

  __asm {
    PUSHAD
    PUSH orderId
    MOV EAX, target
    MOV ESI, this
    CALL Func_OrderToUnit
    POPAD
  }
}

const u32 Func_OrderToPos = 0x00475260; //AKA orderTarget(); Primarily for use with orderNewUnitToRally(), but may have other uses.
void CUnit::orderTo(u32 orderId, u16 x, u16 y) {
  assert(this);
  const u32 x_ = x, y_ = y;

  __asm {
    PUSHAD
    PUSH y_
    PUSH x_
    PUSH orderId
    MOV ESI, this
    CALL Func_OrderToPos
    POPAD
  }
}

const u32 Func_HasPathToTarget = 0x0049CBB0; //AKA unitHasPathToUnit()
bool CUnit::hasPathToUnit(const CUnit *target) const {
  assert(this);
  assert(target);
  u32 result;

  __asm {
    PUSHAD
    MOV EAX, this
    MOV ECX, target
    CALL Func_HasPathToTarget
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

const u32 Func_HasPathToPos = 0x0049CB60; //AKA unitHasPathToDest()
bool CUnit::hasPathToPos(u32 x, u32 y) const {
  assert(this);
  u32 result;

  __asm {
    PUSHAD
    PUSH y
    PUSH x
    MOV EAX, this
    CALL Func_HasPathToPos
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

u32 CUnit::getDistanceToTarget(const CUnit *target) const {
  using Unit::BaseProperty;
  assert(this);
  assert(target);

  const CUnit *unit = this;
  if (BaseProperty[this->id] & UnitProperty::Subunit)
    unit = this->subunit; // Current unit is a turret, so use it's base instead

  s32 dx = unit->getLeft() - target->getRight() - 1;
  if (dx < 0) {
    dx = target->getLeft() - unit->getRight() - 1;
    if (dx < 0)
      dx = 0;
  }

  s32 dy = unit->getTop() - target->getBottom() - 1;
  if (dy < 0) {
    dy = target->getTop() - unit->getBottom() - 1;
    if (dy < 0)
      dy = 0;
  }

  return scbw::getDistanceFast(0, 0, dx, dy);
}

const u32 Func_GetMaxWeaponRange = 0x00475870;
u32 CUnit::getMaxWeaponRange(u8 weaponId) const {
  assert(this);
  assert(weaponId < WEAPON_TYPE_COUNT);

  static u32 maxWeaponRange;
  __asm {
    PUSHAD
    MOV BL, weaponId
    MOV EAX, this
    CALL Func_GetMaxWeaponRange
    MOV maxWeaponRange, EAX
    POPAD
  }

  return maxWeaponRange;
}

const u32 Func_GetSightRange = 0x004E5B40;
u32 CUnit::getSightRange(bool applyStatusEffects) const {
  assert(this);

  static u32 sightRange;
  Bool32 ignoreStatusEffects = (applyStatusEffects ? 0 : 1);
  __asm {
    PUSHAD
    PUSH ignoreStatusEffects
    MOV EDX, this
    CALL Func_GetSightRange
    MOV sightRange, EAX
    POPAD
  }

  return sightRange;
}

const u32 Func_GetMaxEnergy = 0x00491870;
u16 CUnit::getMaxEnergy() const {
  assert(this);
  
  static u16 result;
  __asm {
    PUSHAD
    MOV EAX, this
    CALL Func_GetMaxEnergy
    MOV result, AX
    POPAD
  }

  return result;
}

u8 CUnit::getArmor() const {
  assert(this);
  return Unit::ArmorAmount[this->id] + this->getArmorBonus();
}

const u32 Func_GetArmorBonus = 0x00453FC0;
u8 CUnit::getArmorBonus() const {
  assert(this);

  static u8 armorBonus;
  __asm {
    PUSHAD
    MOV EAX, this
    CALL Func_GetArmorBonus
    MOV armorBonus, AL
    POPAD
  }

  return armorBonus;
}

const u32 Func_UpdateSpeed = 0x00454310;
void CUnit::updateSpeed() {
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    CALL Func_UpdateSpeed
    POPAD
  }
}

void CUnit::playIscriptAnim(IscriptAnimation::Enum animation) {
  assert(this);
  this->sprite->playIscriptAnim(animation);
}

const u32 Func_RemoveLockdown = 0x00454D90;
void CUnit::removeLockdown() {
  assert(this);
  __asm {
    PUSHAD
    MOV ESI, this
    CALL Func_RemoveLockdown
    POPAD
  }
}

const u32 Func_RemoveMaelstrom = 0x00454D20;
void CUnit::removeMaelstrom() {
  assert(this);
  __asm {
    PUSHAD
    MOV ESI, this
    CALL Func_RemoveMaelstrom
    POPAD
  }
}

const u32 Func_RemoveStasisField = 0x004F62D0;
void CUnit::removeStasisField() {
  assert(this);
  __asm {
    PUSHAD
    MOV ESI, this
    CALL Func_RemoveStasisField
    POPAD
  }
}

void CUnit::removeOverlay(u32 imageIdStart, u32 imageIdEnd) {
  assert(this);

  this->sprite->removeOverlay(imageIdStart, imageIdEnd);
  if (this->subunit)
    this->subunit->sprite->removeOverlay(imageIdStart, imageIdEnd);
}

void CUnit::removeOverlay(u32 imageId) {
  assert(this);

  this->sprite->removeOverlay(imageId);
  if (this->subunit)
    this->subunit->sprite->removeOverlay(imageId);
}

u16 CUnit::getX() const {
  assert(this);
  assert(this->sprite);
  return this->sprite->position.x;
};

u16 CUnit::getY() const {
  assert(this);
  assert(this->sprite);
  return this->sprite->position.y;
};

u16 CUnit::getTileX() const {
  assert(this);
  return this->getX() / 32;
};

u16 CUnit::getTileY() const {
  assert(this);
  return this->getY() / 32;
};


s16 CUnit::getLeft() const {
  assert(this);
  return this->getX() - Unit::UnitBounds[this->id].left;
}

s16 CUnit::getRight() const {
  assert(this);
  return this->getX() + Unit::UnitBounds[this->id].right;
}

s16 CUnit::getTop() const {
  assert(this);
  return this->getY() - Unit::UnitBounds[this->id].top;
}

s16 CUnit::getBottom() const {
  assert(this);
  return this->getY() + Unit::UnitBounds[this->id].bottom;
}
