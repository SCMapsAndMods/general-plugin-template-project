#include <cassert>
#include "CUnit.h"
#include "../api.h"
#include "../enumerations.h"

void CUnit::setHp(s32 hitPoints) {
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    MOV ECX, hitPoints
    CALL [offsets::Func_SetUnitHp]
    POPAD
  }
}

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
    CALL offsets::Func_DamageUnitHp
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
    scbw::removeOverlays(this, ImageId::DefensiveMatrixFront_Small,
                         ImageId::DefensiveMatrixFront_Large);
    scbw::removeOverlays(this, ImageId::DefensiveMatrixBack_Small,
                         ImageId::DefensiveMatrixBack_Large);
  }
  if (this->defensiveMatrixTimer && !(this->status & UnitStatus::Burrowed)) {
    CUnit *unit = this->subunit ? this->subunit : this;
    scbw::createOverlay(unit->sprite, scbw::getUnitOverlayAdjustment(unit) + ImageId::DefensiveMatrixHit_Small);
  }
}

void CUnit::remove() {
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    CALL offsets::Func_RemoveUnit
    POPAD
  }
}

void CUnit::orderTo(u32 orderId, const CUnit *target) {
  assert(this);
  assert(target);

  __asm {
    PUSHAD
    PUSH orderId
    MOV EAX, target
    MOV ESI, this
    CALL offsets::Func_OrderToUnit
    POPAD
  }
}

void CUnit::orderTo(u32 orderId, u16 x, u16 y) {
  assert(this);
  const u32 x_ = x, y_ = y;

  __asm {
    PUSHAD
    PUSH y_
    PUSH x_
    PUSH orderId
    MOV ESI, this
    CALL offsets::Func_OrderToPos
    POPAD
  }
}

bool CUnit::hasPathToUnit(const CUnit *target) {
  assert(this);
  assert(target);
  u32 result;

  __asm {
    PUSHAD
    MOV EAX, this
    MOV ECX, target
    CALL offsets::Func_HasPathToTarget
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

bool CUnit::hasPathToPos(u32 x, u32 y) {
  assert(this);
  u32 result;

  __asm {
    PUSHAD
    PUSH y
    PUSH x
    MOV EAX, this
    CALL offsets::Func_HasPathToPos
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

  return scbw::getDistanceFast(0, dx, 0, dy);
}

void CUnit::updateSpeed() {
  assert(this);

  __asm {
    PUSHAD
    MOV EAX, this
    CALL offsets::Func_UpdateSpeed
    POPAD
  }
}

void CUnit::playIscriptAnim(u8 animation) {
  assert(this);
  assert(this->sprite);

  // Based on StarCraft.exe's internal code
  for (CImage *i = this->sprite->underlay; i; i = i->next)
    scbw::playIscriptAnim(i, animation);
}

void CUnit::removeLockdown() {
  assert(this);
  __asm {
    PUSHAD
    MOV ESI, this
    CALL offsets::Func_RemoveLockdown
    POPAD
  }
}

void CUnit::removeMaelstrom() {
  assert(this);
  __asm {
    PUSHAD
    MOV ESI, this
    CALL offsets::Func_RemoveMaelstrom
    POPAD
  }
}

void CUnit::removeStasisField() {
  assert(this);
  __asm {
    PUSHAD
    MOV ESI, this
    CALL offsets::Func_RemoveStasisField
    POPAD
  }
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
