#include "api.h"
#include "scbwdata.h"
#include <SCBW/UnitFinder.h>
#include <algorithm>
#include <cassert>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


namespace scbw {

const u32 Func_PrintText = 0x0048CD30;
void printText(const char* text, u32 color) {
  if (!text) return;
  DWORD gtc = GetTickCount() + 7000;

  __asm   {
    PUSHAD
    PUSH 0  ;//unknown
    MOV eax, text
    PUSH gtc
    PUSH color
    CALL Func_PrintText
    POPAD
  }
}

const u32 Func_PlaySound = 0x0048ED50;
void playSound(u32 sfxId, const CUnit *sourceUnit) {
  __asm {
    PUSHAD
    PUSH 0
    PUSH 1
    MOV ESI, sourceUnit
    MOV EBX, sfxId
    CALL Func_PlaySound
    POPAD
  }
}

const u32 Func_ShowErrorMessageWithSfx = 0x0048EE30;
void showErrorMessageWithSfx(u32 playerId, u32 statTxtId, u32 sfxId) {
  __asm {
    PUSHAD
    MOV ESI, sfxId
    MOV EDI, statTxtId
    MOV EBX, playerId
    CALL Func_ShowErrorMessageWithSfx
    POPAD
  }
}

// Logically equivalent to function @ 0x004C36F0
const char* getStatTxtTblString(u16 index) {
  if (index == 0)
    return NULL;
  else if (index <= **statTxtTbl)
    return (char*)(*statTxtTbl) + (*statTxtTbl)[index];
  else
    return "";
}

u32 getUnitOverlayAdjustment(const CUnit* const unit) {
  if (Unit::BaseProperty[unit->id] & UnitProperty::MediumOverlay)
    return 1;
  else if (Unit::BaseProperty[unit->id] & UnitProperty::LargeOverlay)
    return 2;
  else
    return 0;
}

//-------- Weapon related --------//

//Identical to function @ 0x00475CE0
bool canWeaponTargetUnit(u8 weaponId, const CUnit *target, const CUnit *attacker) {
  if (weaponId >= WEAPON_TYPE_COUNT)
    return false;

  if (target == NULL)
    return Weapon::TargetFlags[weaponId].terrain;

  if (target->status & UnitStatus::Invincible)
    return false;

  const TargetFlag tf = Weapon::TargetFlags[weaponId];
  const u32 targetProps = Unit::BaseProperty[target->id];

  if ((target->status & UnitStatus::InAir) ? !tf.air : !tf.ground)
    return false;

  if (tf.mechanical && !(targetProps & UnitProperty::Mechanical))
    return false;

  if (tf.organic && !(targetProps & UnitProperty::Organic))
    return false;

  if (tf.nonBuilding && (targetProps & UnitProperty::Building))
    return false;

  if (tf.nonRobotic && (targetProps & UnitProperty::RoboticUnit))
    return false;

  if (tf.orgOrMech && !(targetProps & (UnitProperty::Organic | UnitProperty::Mechanical)))
    return false;

  if (tf.playerOwned && target->playerId != attacker->playerId)
    return false;

  return true;
}

const u32 Func_CreateUnitAtPos = 0x004CD360; //AKA createUnitXY()
CUnit* createUnitAtPos(u16 unitType, u16 playerId, u32 x, u32 y) {
  if (unitType >= UNIT_TYPE_COUNT) return NULL;
  CUnit* unit;

  __asm {
    PUSHAD
    MOV CX, unitType
    MOV AX, playerId
    PUSH y
    PUSH x
    CALL Func_CreateUnitAtPos
    MOV unit, EAX
    POPAD
  }

  return unit;
}

const u32 Func_CanBeEnteredBy = 0x004E6E00; //AKA CanEnterTransport()
bool canBeEnteredBy(const CUnit* transport, const CUnit* unit) {
  u32 result;

  __asm {
    PUSHAD
    MOV EAX, transport
    PUSH unit
    CALL Func_CanBeEnteredBy
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

//-------- isUnderDarkSwarm() --------//

class DarkSwarmFinderProc: public UnitFinderCallbackMatchInterface {
  public:
    bool match(const CUnit *unit) {
      return unit->id == UnitId::Spell_DarkSwarm;
    }
};

bool isUnderDarkSwarm(const CUnit *unit) {
  static UnitFinder darkSwarmFinder;
  static DarkSwarmFinderProc dsFinder;
  darkSwarmFinder.search(unit->getLeft(), unit->getTop(), unit->getRight(), unit->getBottom());
  return darkSwarmFinder.getFirst(dsFinder) != NULL;
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

//Identical to function @ 0x00494C10
int arctangent(int slope) {
  const unsigned int tangentTable[] = {
       7,   13,   19,   26,   32,   38,   45,   51,
      58,   65,   71,   78,   85,   92,   99,  107, 
     114,  122,  129,  137,  146,  154,  163,  172,
     181,  190,  200,  211,  221,  233,  244,  256,
     269,  283,  297,  312,  329,  346,  364,  384,
     405,  428,  452,  479,  509,  542,  578,  619,
     664,  716,  775,  844,  926, 1023, 1141, 1287,
    1476, 1726, 2076, 2600, 3471, 5211, 10429, -1
  };

  bool isNegative = false;
  if (slope < 0) {
    isNegative = true;
    slope = -slope;
  }

  int min = 0, max = 64, angle = 32;

  do {
    if ((unsigned int) slope <= tangentTable[angle])
      max = angle;
    else
      min = angle + 1;
    angle = (min + max) / 2;
  } while (min != max);

  return (isNegative ? -angle : angle);
}

//Identical to function @ 0x00495300
s32 getAngle(s32 xHead, s32 yHead, s32 xTail, s32 yTail) {
  s32 dx = xHead - xTail, dy = yHead - yTail;
  
  if (dx == 0)
    return dy > 0 ? 128 : 0;

  s32 angle = arctangent((dy << 8) / dx);
  if (dx < 0) {
    angle += 192;
    return angle == 256 ? 0 : angle;
  }
  else
    return angle + 64;
}

u8 getUpgradeLevel(u8 playerId, u8 upgradeId) {
  using Upgrade::UpgSc;
  using Upgrade::UpgBw;

  assert(playerId < PLAYER_COUNT);
  assert(upgradeId < UpgradeId::None);

  if (upgradeId < UpgradeId::UnusedUpgrade46)
    return Upgrade::UpgSc->currentLevel[playerId][upgradeId];
  else
    return Upgrade::UpgBw->currentLevel[playerId][upgradeId - UpgradeId::UnusedUpgrade46];
}

void setUpgradeLevel(u8 playerId, u8 upgradeId, u8 level) {
  using Upgrade::UpgSc;
  using Upgrade::UpgBw;

  assert(playerId < PLAYER_COUNT);
  assert(upgradeId < UpgradeId::None);

  if (upgradeId < UpgradeId::UnusedUpgrade46)
    Upgrade::UpgSc->currentLevel[playerId][upgradeId] = level;
  else
    Upgrade::UpgBw->currentLevel[playerId][upgradeId - UpgradeId::UnusedUpgrade46] = level;
}

bool hasTechResearched(u8 playerId, u16 techId) {
  using Tech::TechSc;
  using Tech::TechBw;

  assert(playerId < PLAYER_COUNT);
  assert(techId < TechId::None);

  if (techId < TechId::Restoration)
    return TechSc->isResearched[playerId][techId] != 0;
  else
    return TechBw->isResearched[playerId][techId - TechId::Restoration] != 0;
}

void setTechResearchState(u8 playerId, u16 techId, bool isResearched) {
  using Tech::TechSc;
  using Tech::TechBw;

  assert(playerId < PLAYER_COUNT);
  assert(techId < TechId::None);

  if (techId < TechId::Restoration)
    TechSc->isResearched[playerId][techId] = isResearched;
  else
    TechBw->isResearched[playerId][techId - TechId::Restoration] = isResearched;
}

s32 getSupplyRemaining(u8 playerId, u8 raceId) {
  assert(raceId <= 2);
  assert(playerId < 12);

  s32 supplyProvided;
  if (isCheatEnabled(CheatFlags::FoodForThought))
    supplyProvided = raceSupply[raceId].max[playerId];
  else
    supplyProvided = std::min(raceSupply[raceId].max[playerId], raceSupply[raceId].provided[playerId]);
  return supplyProvided - raceSupply[raceId].used[playerId];
}

u8 getRaceId(u16 unitId) {
  assert(unitId < UNIT_TYPE_COUNT);
  GroupFlag ugf = Unit::GroupFlags[unitId];
  if (ugf.isZerg)
    return 0;
  else if (ugf.isTerran)
    return 1;
  else if (ugf.isProtoss)
    return 2;
  else
    return 4;
}

const u32 Func_GetGroundHeightAtPos = 0x004BD0F0;
u32 getGroundHeightAtPos(s32 x, s32 y) {
  u32 height;

  __asm {
    PUSHAD
    MOV EAX, y
    MOV ECX, x
    CALL Func_GetGroundHeightAtPos
    MOV height, EAX
    POPAD
  }

  return height;
}

void refreshScreen(int left, int top, int right, int bottom) {
  left  >>= 4; right  = (right  + 15) >> 4;
  top   >>= 4; bottom = (bottom + 15) >> 4;

  if (left > right) std::swap(left, right);
  if (top > bottom) std::swap(top, bottom);

  //Rect out of bounds
  if (left >= 40 || right < 0 || top >= 30 || bottom < 0) return;

  left  = std::max(left,  0); right   = std::min(right,   40 - 1);
  top   = std::max(top,   0); bottom  = std::min(bottom,  30 - 1);

  for (int y = top; y <= bottom; ++y)
    memset(&refreshRegions[40 * y + left], 1, right - left + 1);
}

void refreshScreen() {
  memset(refreshRegions, 1, 1200);
}

//Logically equivalent to function @ 0x004C36C0
void refreshConsole() {
  u32*  const bCanUpdateCurrentButtonSet      = (u32*)  0x0068C1B0;
  u8*   const bCanUpdateSelectedUnitPortrait  = (u8*)   0x0068AC74;
  u8*   const bCanUpdateStatDataDialog        = (u8*)   0x0068C1F8;
  u32*  const someDialogUnknown               = (u32*)  0x0068C1E8;
  u32*  const unknown2                        = (u32*)  0x0068C1EC;

  *bCanUpdateCurrentButtonSet = 1;
  *bCanUpdateSelectedUnitPortrait = 1;
  *bCanUpdateStatDataDialog = 1;
  *someDialogUnknown = 0;
  *unknown2 = 0;
}

u16 random() {
  if (*IS_IN_GAME_LOOP) {
    *lastRandomNumber = 22695477 * (*lastRandomNumber) + 1;
    return (*lastRandomNumber >> 16) % 32768;  //Make a number between 0 and 32767
  }
  else
    return 0;
}

u32 randBetween(u32 min, u32 max) {
  assert(min <= max);
  return min + ((max - min + 1) * random() >> 15);
}

} //scbw
