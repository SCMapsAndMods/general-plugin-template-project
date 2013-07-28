#include "api.h"
#include "offsets.h"
#include "scbwdata.h"
#include <algorithm>
#include <cassert>


namespace scbw {

void printText(char* text, DWORD color) {
  if (!text) return;
  DWORD gtc = GetTickCount() + 7000;

  __asm   {
    PUSHAD
    PUSH 0  ;//unknown
    MOV eax, text
    PUSH gtc
    PUSH color
    CALL offsets::Func_PrintText
    POPAD
  }
}

void playSound(u32 sfxId, const CUnit *sourceUnit) {
  __asm {
    PUSHAD
    PUSH 0
    PUSH 1
    MOV ESI, sourceUnit
    MOV EBX, sfxId
    CALL offsets::Func_PlaySound
    POPAD
  }
}

void doWeaponDamage(DWORD   damage,
                    CUnit*  target,
                    BYTE    weaponId,
                    CUnit*  attacker,
                    DWORD   attackingPlayer,
                    BYTE    direction,
                    DWORD   dmgDivisor
                    ) {
  if (!target || weaponId >= WEAPON_TYPE_COUNT || dmgDivisor == 0) return;

  DWORD _direction = direction;
  DWORD _weaponId = weaponId;

  __asm {
    PUSHAD
    MOV edi, target
    MOV eax, damage
    PUSH attackingPlayer
    PUSH attacker
    PUSH _direction
    PUSH dmgDivisor
    PUSH _weaponId
    CALL [offsets::Func_DoWeaponDamage]
    POPAD
  }
}

void createOverlay(CSprite* sprite, DWORD imageId, BYTE x, BYTE y, DWORD direction) {
  if (!sprite) return;
  DWORD _x = x, _y = y;

  __asm {
    PUSHAD
    PUSH direction
    PUSH _y
    PUSH _x
    MOV esi, imageId
    MOV eax, sprite
    CALL [offsets::Func_CreateOverlay]
    POPAD
  }
}

void removeOverlays(CUnit *unit, DWORD imageIdStart, DWORD imageIdEnd) {
  if (!unit) return;
  if (imageIdStart > imageIdEnd) {
    DWORD tmp = imageIdStart;
    imageIdStart = imageIdEnd;
    imageIdEnd = tmp;
  }

  __asm {
    PUSHAD
    MOV eax, unit
    MOV edx, imageIdStart
    MOV edi, imageIdEnd
    CALL [offsets::Func_RemoveOverlays]
    POPAD
  }
}

// Logically equivalent to function @ 0x004E5C90
bool hasOverlay(const CUnit* const unit, const u32 imageId) {
  CImage *image = unit->sprite->underlay;
  while (image) {
    if (image->id == imageId)
      return true;
    image = image->next;
  }
  if (unit->subunit) {
    image = unit->subunit->sprite->underlay;
    while (image) {
      if (image->id == imageId)
        return true;
      image = image->next;
    }
  }
  return false;
}

u32 getUnitOverlayAdjustment(const CUnit* const unit) {
  if (Unit::BaseProperty[unit->id] & UnitProperty::MediumOverlay)
    return 1;
  else if (Unit::BaseProperty[unit->id] & UnitProperty::LargeOverlay)
    return 2;
  else
    return 0;
}

void fireUnitWeapon(CUnit* unit, u8 weaponId) {
  if (weaponId >= WEAPON_TYPE_COUNT) return;
  u32 _weaponId = weaponId;

  __asm {
    PUSHAD
    PUSH _weaponId
    MOV ESI, unit
    CALL offsets::Func_FireUnitWeapon
    POPAD
  }
}

void playIscriptAnim(CImage* image, u8 animation) {
  if (!image) return;
  u32 animation_ = animation;

  __asm {
    PUSHAD
    PUSH animation_
    MOV ECX, image
    CALL offsets::Func_PlayIscriptAnim
    POPAD
  }
}

CUnit* createUnitAtPos(u16 unitType, u16 playerId, u32 x, u32 y) {
  if (unitType >= UNIT_TYPE_COUNT) return NULL;
  CUnit* unit;

  __asm {
    PUSHAD
    MOV CX, unitType
    MOV AX, playerId
    PUSH y
    PUSH x
    CALL offsets::Func_CreateUnitAtPos
    MOV unit, EAX
    POPAD
  }

  return unit;
}

bool canBeEnteredBy(const CUnit* transport, const CUnit* unit) {
  u32 result;

  __asm {
    PUSHAD
    MOV EAX, transport
    PUSH unit
    CALL offsets::Func_CanBeEnteredBy
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

// Improved code from BWAPI's include/BWAPI/Position.h: getApproxDistance()
// Logically same as function @ 0x0040C360
u32 getDistanceFast(s32 x1, s32 y1, s32 x2, s32 y2) {
  int dMax = abs(x1 - x2), dMin = abs(y1 - y2);
  if (dMax < dMin)
    std::swap(dMax, dMin);

  if (dMin <= (dMax >> 2))
    return dMax;

  return (dMin * 3 >> 3) + (dMin * 3 >> 8) + dMax - (dMax >> 4) - (dMax >> 6);
}

u8 getUpgradeLevel(const u8 playerId, const u8 upgradeId) {
  assert(playerId < PLAYER_COUNT);
  assert(upgradeId < 61);
  if (upgradeId < 46)
    return Upgrade::CurrentUpgSc->level[playerId][upgradeId];
  else
    return Upgrade::CurrentUpgBw->level[playerId][upgradeId - 46];
}

u32 getGroundHeightAtPos(s32 x, s32 y) {
  u32 height;

  __asm {
    PUSHAD
    MOV EAX, y
    MOV ECX, x
    CALL offsets::Func_GetGroundHeightAtPos
    MOV height, EAX
    POPAD
  }

  return height;
}

//---------------- Unit Finder stuff ----------------//

//Based on BWAPI's Shared/Templates.h
int findUnitsInBounds(s32 left, s32 right, s32 top, s32 bottom,
                         CUnit** unitsFound) {
  u32 finderFlags[UNIT_ARRAY_LENGTH + 1] = {0}; //1701 elements

  int r = right, b = bottom;
  bool isWidthExtended  = right - left + 1 < *maxUnitWidth;
  bool isHeightExtended = top - bottom + 1 < *maxUnitHeight;

  // Check if the location is smaller than the largest unit
  if (isWidthExtended)
    r += *maxUnitWidth;
  if (isHeightExtended)
    b += *maxUnitHeight;

  // Obtain finder indexes for all bounds
  UnitFinderData *p_xend = unitOrderingX + *unitOrderingCount;
  UnitFinderData *p_yend = unitOrderingY + *unitOrderingCount;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.searchValue = left;
  UnitFinderData *pLeft   = std::lower_bound<UnitFinderData*>(unitOrderingX, p_xend, finderVal);

  finderVal.searchValue = top;
  UnitFinderData *pTop    = std::lower_bound<UnitFinderData*>(unitOrderingY, p_yend, finderVal);

  finderVal.searchValue = r + 1;
  UnitFinderData *pRight  = std::upper_bound<UnitFinderData*>(pLeft, p_xend, finderVal);

  finderVal.searchValue = b + 1;
  UnitFinderData *pBottom = std::upper_bound<UnitFinderData*>(pTop, p_yend, finderVal);

  // Iterate the X entries of the finder
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    int iUnitIndex = px->unitIndex;
    if (finderFlags[iUnitIndex] == 0) {
      if (isWidthExtended) {  // If width is small, check unit bounds
        CUnit *unit = &unitTable[iUnitIndex - 1];
        if (unit->getLeft() <= right)
          finderFlags[iUnitIndex] = 1;
      }
      else
        finderFlags[iUnitIndex] = 1;
    }
  }

  // Iterate the Y entries of the finder
  for (UnitFinderData *py = pTop; py < pBottom; ++py) {
    int iUnitIndex = py->unitIndex;
    if (finderFlags[iUnitIndex] == 1) {
      if (isHeightExtended) { // If height is small, check unit bounds
        CUnit *unit = &unitTable[iUnitIndex - 1];
        if (unit->getTop() <= bottom)
          finderFlags[iUnitIndex] = 2;
      }
      else
        finderFlags[iUnitIndex] = 2;
    }
  }

  // Final iteration
  int unitsFoundCount = 0;
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    int iUnitIndex = px->unitIndex;
    if (finderFlags[iUnitIndex] == 2) {
      CUnit *unit = &unitTable[iUnitIndex - 1];
      if (unit && unit->mainOrderId)
        unitsFound[unitsFoundCount++] = unit;
    }
  }

  return unitsFoundCount;
}

} //scbw
