#include <cassert>
#include "CUnit.h"
#include "../api.h"
#include "../enumerations.h"

RaceId::Enum CUnit::getRace() const {
  assert(this);
  return getRace(this->id);
}

//Based on function at 0x0047B150
RaceId::Enum CUnit::getRace(u16 unitId) {
  assert(unitId < UNIT_TYPE_COUNT);

  if (units_dat::GroupFlags[unitId].isZerg)
    return RaceId::Zerg;
  else if (units_dat::GroupFlags[unitId].isProtoss)
    return RaceId::Protoss;
  else if (units_dat::GroupFlags[unitId].isTerran)
    return RaceId::Terran;
  else
    return RaceId::Neutral;
}

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

extern const u32 Func_DoWeaponDamage = 0x00479930; //Note: Also used by weaponDamageHook()
void CUnit::damageWith(s32 damage, u8 weaponId, CUnit *attacker,
                       u8 attackingPlayer, s8 direction, u8 damageDivisor) {
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
    CALL Func_DoWeaponDamage
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

//-------- Unit orders --------//

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

//Logically equivalent to function @ 0x004752B0
void CUnit::orderTo(u8 orderId, const CUnit *target) {
  assert(this);

  this->userActionFlags |= 0x1;
  if (target)
    this->order(orderId, target->getX(), target->getY(), target, UnitId::None, true);
  else
    this->order(orderId, 0, 0, target, UnitId::None, true);
  
  prepareForNextOrder(this);
}

//Logically equivalent to function @ 0x00475260
void CUnit::orderTo(u8 orderId, u16 x, u16 y) {
  assert(this);

  this->userActionFlags |= 0x1;
  this->order(orderId, x, y, NULL, UnitId::None, true);
  prepareForNextOrder(this);
}

//Logically equivalent to function @ 0x004753A0
void CUnit::orderToIdle() {
  assert(this);

  if (this->orderQueueHead) {
    this->userActionFlags |= 1;
    prepareForNextOrder(this);
  }
  else {
    if (this->pAI)
      this->orderTo(OrderId::ComputerAI);
    else
      this->orderTo(units_dat::ReturnToIdleOrder[this->id]);
  }
}

const u32 Func_Order = 0x00474810;
void CUnit::order(u8 orderId, u16 x, u16 y, const CUnit *target, u16 targetUnitId, bool stopPreviousOrders) {
  assert(this);
  static Point16 pos;
  static u32 targetUnitId_;
  pos.x = x, pos.y = y;
  targetUnitId_ = targetUnitId;

  __asm {
    PUSHAD
    PUSH targetUnitId_
    PUSH target
    PUSH pos
    MOVZX EAX, stopPreviousOrders
    MOV CL, orderId
    MOV EDX, this
    CALL Func_Order
    POPAD
  }
}

//Identical to @ 0x004743D0
void CUnit::setSecondaryOrder(u8 orderId) {
  assert(this);
  if (this->secondaryOrderId == orderId)
    return;
  this->secondaryOrderId = orderId;
  this->secondaryOrderPos.x = 0;
  this->secondaryOrderPos.y = 0;
  this->currentBuildUnit = NULL;
  this->secondaryOrderState = 0;
}

//-------- End of unit orders --------//

extern const u32 Func_FireUnitWeapon = 0x00479C90;

void CUnit::fireWeapon(u8 weaponId) const {
  assert(this);
  static u32 weaponId_ = weaponId;
  __asm {
    PUSHAD
    PUSH weaponId_
    MOV ESI, this
    CALL Func_FireUnitWeapon
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

const u32 Func_CanUseTech = 0x0046DD80;
int CUnit::canUseTech(u8 techId, u8 playerId) const {
  assert(this);
  s32 playerId_ = playerId;
  static s32 result;

  __asm {
    PUSHAD
    PUSH playerId_
    MOVZX DI, techId
    MOV EAX, this
    CALL Func_CanUseTech
    MOV result, EAX
    POPAD
  }

  return result;
}

const u32 Func_CanBuild = 0x0046E1C0;
int CUnit::canBuild(u16 unitId, u8 playerId) const {
  assert(this);
  s32 playerId_ = playerId;
  static s32 result;

  __asm {
    PUSHAD
    PUSH playerId_
    MOV AX, unitId
    MOV ESI, this
    CALL Func_CanBuild
    MOV result, EAX
    POPAD
  }

  return result;
}

extern const u32 Func_CanDetect = 0x00403430;
bool CUnit::canDetect() const {
  assert(this);
  static u32 result;

  __asm {
    PUSHAD
    MOV EAX, this
    CALL Func_CanDetect
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

//Identical to function @ 0x004020B0
bool CUnit::isFrozen() const {
  assert(this);
  return this->status & UnitStatus::DoodadStatesThing
    || this->lockdownTimer || this->stasisTimer || this->maelstromTimer;
}

//Identical to function @ 0x00401210
bool CUnit::isValidCaster() const {
  assert(this);
  return units_dat::BaseProperty[this->id] & UnitProperty::Spellcaster
    && !(this->status & UnitStatus::IsHallucination);
}

extern const u32 Func_IsRemorphingBuilding = 0x0045CD00;
bool CUnit::isRemorphingBuilding() const {
  static Bool32 result;
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    CALL Func_IsRemorphingBuilding
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

u32 CUnit::getDistanceToTarget(const CUnit *target) const {
  using units_dat::BaseProperty;
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

extern const u32 Func_GetMaxWeaponRange = 0x00475870;
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

extern const u32 Func_GetSeekRange = 0x00476000;
u8 CUnit::getSeekRange() const {
  assert(this);

  static u8 seekRange;
  __asm {
    PUSHAD
    MOV EDX, this
    CALL Func_GetSeekRange
    MOV seekRange, AL
    POPAD
  }

  return seekRange;
}

extern const u32 Func_GetSightRange = 0x004E5B40;
u32 CUnit::getSightRange(bool isForSpellCasting) const {
  assert(this);

  static u32 sightRange;
  Bool32 ignoreStatusEffects = (isForSpellCasting ? 0 : 1);
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

extern const u32 Func_GetMaxEnergy = 0x00491870;
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
  return units_dat::ArmorAmount[this->id] + this->getArmorBonus();
}

extern const u32 Func_GetArmorBonus = 0x00453FC0;
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

u8 CUnit::getActiveGroundWeapon() const {
  assert(this);
  if (this->id == UnitId::lurker && !(this->status & UnitStatus::Burrowed))
    return WeaponId::None;
  else
    return units_dat::GroundWeapon[this->id];
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

//Similar to function @ 0x004E5C90
CImage* CUnit::getOverlay(u16 imageId) const {
  assert(this);

  CImage *image = this->sprite->getOverlay(imageId);
  if (!image && this->subunit)
    image = this->subunit->sprite->getOverlay(imageId);

  return image;
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
  return this->getX() - units_dat::UnitBounds[this->id].left;
}

s16 CUnit::getRight() const {
  assert(this);
  return this->getX() + units_dat::UnitBounds[this->id].right;
}

s16 CUnit::getTop() const {
  assert(this);
  return this->getY() - units_dat::UnitBounds[this->id].top;
}

s16 CUnit::getBottom() const {
  assert(this);
  return this->getY() + units_dat::UnitBounds[this->id].bottom;
}

const char* CUnit::getName() const {
  assert(this);
  return scbw::getStatTxtTblString(this->id + 1);
}

CUnit* CUnit::getFromIndex(u16 index) {
  if (1 <= index && index <= UNIT_ARRAY_LENGTH)
    return &unitTable[index - 1];
  return NULL;
}

u16 CUnit::getIndex() const {
  return this - unitTable;
}

u8 CUnit::getLastOwnerId() const {
  assert(this);
  assert(this->sprite);
  if (this->playerId == 11) //The owner has left the game
    return this->sprite->playerId;
  else
    return this->playerId;
}

bool CUnit::isVisibleTo(u8 playerId) const {
  assert(this);
  return (this->visibilityStatus & (1 << playerId)) != 0;
}

CUnit* CUnit::getLoadedUnit(int index) const {
  assert(0 <= index && index < 8);

  CUnit *loadedUnit = CUnit::getFromIndex(this->loadedUnit[index].index);
  if (loadedUnit && loadedUnit->sprite
      && !(loadedUnit->mainOrderId == OrderId::Die && loadedUnit->mainOrderState == 1)
      && loadedUnit->targetOrderSpecial == this->loadedUnit[index].unitId)
    return loadedUnit;

  return NULL;
}

//Logically equivalent to function @ 0x004E7110
bool CUnit::hasLoadedUnit() const {
  assert(this);

  for (int i = 0; i < 8; ++i)
    if (!getLoadedUnit(i))
      return true;

  return false;
}
