//All-in-one include file
//Structs by poiuy_qwert, A_of_s_t, ravenwolf and pastelmind.
//Special thanks to Heinermann

#pragma once
#include "structures/CUnit.h"
#include "structures/CBullet.h"
#include "structures/CSprite.h"
#include "structures/CImage.h"
#include "structures/COrder.h"
#include "structures/Layer.h"

#pragma pack(1)

struct UNITDEATHS {
  u32        player[12];
};

//Based on http://code.google.com/p/bwapi/source/browse/branches/bwapi4/bwapi/BWAPI/Source/BW/CImage.h

struct TEXT {
  char       textdisplay[218][13];  //[last is text at bottom; ex: "Cheat enabled."]
  UNK        unknown1;
  UNK        unknown2;
  u8         color[13];
  UNK        unknown3[3];  //[last is text at bottom; ex: "Cheat enabled."]
  u8         timer[13];
};

struct TILE {
      u16 index;
      u8 buildability; /**< 8th bit should sign not buildable. */
      u8 groundHeight; /**< Ground Height(4 lower bits) - Deprecated? Some values are incorrect. */
      u16 leftEdge;
      u16 topEdge;
      u16 rightEdge;
      u16 buttomEdge;
      u16 _1;
      u16 _2; /**<  Unknown - Edge piece has rows above it. (Recently noticed; not fully understood.)
                    o 1 = Basic edge piece.
                    o 2 = Right edge piece.
                    o 3 = Left edge piece.*/
      u16 _3;
      u16 _4;
      u16 miniTile[16]; /** MegaTile References (VF4/VX4) */
};

struct DOODAD {
      u16 index;
      u8 buildability; /**< 8th bit should sign not buildable. */
      u8 groundHeightAndOverlayFlags; /**< Ground Height(4 lower bits) - Deprecated? Some values are incorrect.
                       * Overlay Flags:
                       * o 0x0 - None
                       * o 0x1 - Sprites.dat Reference
                       * o 0x2 - Units.dat Reference (unit sprite)
                       * o 0x4 - Overlay is Flipped
                       */
      u16 overlayID;
      u16 _1;
      u16 doodatGrupString;
      u16 _2;
      u16 dddataBinIndex;
      u16 doodatHeight;
      u16 doodatWidth;
      u16 _3;
      u16 miniTile[16]; /** MegaTile References (VF4/VX4) */
};

//---- Taken from locations.cpp ----//

struct MapSize {
  u16   width;
  u16   height;
};

struct LOCATION {
  u32       topLeftX;
  u32       topLeftY;
  u32       bottomRightX;
  u32       bottomRightY;
  u16       stringId;
  u16       flags;
};

//---- Taken from player.cpp ----//

struct PLAYER {
  u32           id;
  u32           actions; // unused; FF FF FF FF if not a human player
  u8            type;
  u8            race; //Use with Player::Race
  u8            force;
  char          name[25];
};

//---- Taken from buttons.cpp ----//

typedef u32 ButtonState;
typedef ButtonState (__fastcall *REQ_FUNC)(u32, u32, CUnit*);
//(u32 reqVar, u32 playerId, CUnit* selected);
typedef void (__fastcall *ACT_FUNC)(u32,u32);
//(u32 actVar, u32 shiftClick);

struct BUTTON {
  u16        position;
  u16        iconID;
  REQ_FUNC*    reqFunc;
  ACT_FUNC*    actFunc;
  u16        reqVar;
  u16        actVar;
  u16        reqStringID;
  u16        actStringID;
};

struct BUTTON_SET {
  u32        buttonsInSet;
  BUTTON*      firstButton;
  u32        connectedUnit;
};

//---- Taken from resources.cpp ----//

struct RESOURCES {
  int        playerMin[12];
  int        playerGas[12];
  int        cumulativeGas[12];
  int        cumulativeMinerals[12];
};

//---- Taken from triggers.h ----//
struct CONDITION {
  u32        location;
  u32        player;
  u32        number;
  u16        unit;
  u8        comparison;
  u8        condition;
  u8        type;
  u8        flags;
};

typedef Bool32 (__fastcall *ConditionPointer)(CONDITION*);

struct ACTION {
  u32        location;
  u32        string;
  u32        wavString;
  u32        time;
  u32        player;
  u32        number;
  u16        unit;
  u8        action;
  u8        number2;
  u8        flags;
};

typedef Bool32 (__fastcall *ActionPointer)(ACTION*);

//-------- Flag structures --------//

//Based on BWAPI's Offsets.h
struct ActiveTile {
  u8 visibilityFlags;
  u8 exploredFlags;
  u8 isWalkable         : 1; // Set on tiles that can be walked on
  u8 unknown1           : 1; // Unused?
  u8 isUnwalkable       : 1; // Set on tiles that can't be walked on
  u8 unknown2           : 1; // Unused?
  u8 hasDoodadCover     : 1; // Set when a doodad cover (trees) occupies the area
  u8 unknown3           : 1; // Unused?
  u8 hasCreep           : 1; // Set when creep occupies the area
  u8 alwaysUnbuildable  : 1; // always unbuildable, like water
  u8 groundHeight       : 3; // ground height; 1 for low ground, 2 for middle ground and 4 for high ground
  u8 currentlyOccupied  : 1; // unbuildable but can be made buildable
  u8 creepReceeding     : 1; // Set when the nearby structure supporting the creep is destroyed
  u8 cliffEdge          : 1; // Set if the tile is a cliff edge
  u8 temporaryCreep     : 1; // Set when the creep occupying the area was created. Not set if creep tiles were preplaced. Used in drawing routine.
  u8 unknown4           : 1; // Unused?
};

C_ASSERT(sizeof(ActiveTile) == 4);

struct GroupFlag {
  u8 isZerg         : 1;
  u8 isTerran       : 1;
  u8 isProtoss      : 1;
  u8 isMen          : 1;
  u8 isBuilding     : 1;
  u8 isFactory      : 1;
  u8 isIndependent  : 1;
  u8 isNeutral      : 1;
};

C_ASSERT(sizeof(GroupFlag) == 1);

struct TargetFlag {
  u16 air         : 1;
  u16 ground      : 1;
  u16 mechanical  : 1;
  u16 organic     : 1;
  u16 nonBuilding : 1;
  u16 nonRobotic  : 1;
  u16 terrain     : 1;
  u16 orgOrMech   : 1;
  u16 playerOwned : 1;
};

C_ASSERT(sizeof(TargetFlag) == 2);

//-------- End of flag structures --------//

struct UnitFinderData {
  s32 unitIndex;
  s32 searchValue;
  bool operator < (const UnitFinderData& rhs) const {
    return this->searchValue < rhs.searchValue;
  }
};

C_ASSERT(sizeof(UnitFinderData) == 8);

struct Bounds {
  u16 left;
  u16 top;
  u16 right;
  u16 bottom;
  u16 width;
  u16 height;
};

C_ASSERT(sizeof(Bounds) == 12);

struct BinDlg {
  BinDlg  *next;
  Bounds  bounds;
  u8      *buffer;
  char    *pszText;
  u32     flags;
  u32     unk_1c;
  u16     index;
  u16     controlType;
  u16     graphic;
  u32     *user;
  void    *fxnInteract;
  void    *fxnUpdate;
  BinDlg  *parent;
  Box16   responseArea;
  u32     unk_3e;
  void    *childrenSmk;
  Point16 textPos;
  u16     responseAreaWidth;
  u16     responseAreaHeight;
  UNK     unk_4e[8];
};

C_ASSERT(sizeof(BinDlg) == 86);

struct GuiOverlay {
  UNK     unk_0[6];
  u16     overlayType;
  u16     id;
  UNK     unk_a[34];
};

C_ASSERT(sizeof(GuiOverlay) == 44);

#pragma pack()
