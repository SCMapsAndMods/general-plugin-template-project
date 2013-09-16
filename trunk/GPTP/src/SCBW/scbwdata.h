// Contains the definitions of various data structures, arrays, pointers and
// internal constants in StarCraft.exe.
// The contents of this file are merged from: unit.cpp, triggers.cpp,
// resources.cpp, player.cpp, locations.cpp, buttons.cpp, and qdp.cpp.

#pragma once
#include "../definitions.h"
#include "offsets.h"
#include "structures.h"

#pragma pack(1)

#define SCBW_DATA(type, name, offset) type const name = (type)offset;

const int PLAYER_COUNT        = 12;
const int UNIT_ARRAY_LENGTH   = 1700;
const int BULLET_ARRAY_LENGTH = 100;
const int SPRITE_ARRAY_LENGTH = 2500;
const int UNIT_TYPE_COUNT     = 228;
const int TECH_TYPE_COUNT     = 44;
const int UPGRADE_TYPE_COUNT  = 61;
const int WEAPON_TYPE_COUNT   = 130;
const int FLINGY_TYPE_COUNT   = 209;


SCBW_DATA(CUnit*,       unitTable,    0x0059CCA8);
SCBW_DATA(CBullet*,     bulletTable,  0x0064B2E8);
SCBW_DATA(UNITDEATHS*,  deathTable,   0x0058A364);
SCBW_DATA(UNITDEATHS*,  unit_deaths,  deathTable);
SCBW_DATA(CSprite*,     spriteTable,  0x00629D98);

//From resources.cpp
RESOURCES*  const resourceTable   = (RESOURCES*)(0x0057F0F0);
RESOURCES*  const resources       = &resourceTable[0];

//From player.cpp
PLAYER*     const playerTable     = (PLAYER*)(0x0057EEE0);

//From locations.cpp
MapSize*    const mapTileSize     = (MapSize*) 0x0057F1D4;
LOCATION*   const locationTable   = (LOCATION*)(0x0058DC60);

//From buttons.cpp
BUTTON*     const button          = 0;  //???
BUTTON_SET* const buttonSet       = 0;  //???

//From triggers.cpp
ActionPointer*  const actionTable = (ActionPointer*)(0x00512800);

//This was in the InitializePlugin() code in qdp.cpp.
//Not sure what this does, so I'm leaving it as a comment.
//actionTable[59] = &EnableDebugModeAction;

/// The first nodes in StarCraft's internal linked lists (taken from BWAPI's Offsets.h)
CUnit**    const firstVisibleUnit  = (CUnit**) 0x00628430;
/// Includes units in transports/bunkers, workers inside gas buildings, nukes in
/// silos, queens inside command centers, and units in production.
CUnit**    const firstHiddenUnit    = (CUnit**)   0x006283EC;
CUnit**    const firstScannerSweep  = (CUnit**)   0x006283F4;
/// Array of size 12.
CUnit**    const firstPlayerUnit    = (CUnit**)   0x006283F8;
CBullet**  const firstBullet        = (CBullet**) 0x0064DEC4;
SCBW_DATA(CUnit**, firstPsiFieldProvider, 0x0063FF54);

// Units that are selected by the current player (or the player viewing the replay).
CUnit* const* const clientSelectionGroup = (CUnit**) 0x00597208;  //Array of size 12
const u8* const clientSelectionCount = (u8*) 0x0059723D;

//Contains various info on the tiles.
ActiveTile** const activeTileArray  = (ActiveTile**)  0x006D1260;

// Stores an ordered list of all units currently in the game.
UnitFinderData* const unitOrderingX = (UnitFinderData*) 0x0066FF78;
UnitFinderData* const unitOrderingY = (UnitFinderData*) 0x006769B8;

// Font & Drawing
namespace graphics { class Font; class Bitmap; }
graphics::Font**  const fontBase          = (graphics::Font**)  0x006CE0F4;
graphics::Bitmap* const gameScreenBuffer  = (graphics::Bitmap*) 0x006CEFF0;
u8*   const refreshRegions  = (u8*)   0x006CEFF8; //640 x 480 divided into 1200 squares of 16x16
u32*  const screenX         = (u32*)  0x00628448;
u32*  const screenY         = (u32*)  0x00628470;
typedef void (__stdcall *DrawGameProc)(graphics::Bitmap *surface, Bounds *bounds);
static DrawGameProc const oldDrawGameProc = (DrawGameProc) 0x004BD580;
SCBW_DATA(const Point32*, mouse, 0x006CDDC4);
SCBW_DATA(Layers*, screenLayers, 0x006CEF50);

SCBW_DATA(CImage**, firstUnusedImage, 0x0057EB68);
SCBW_DATA(CImage**, lastUnusedImage, 0x0057EB70);
const CListExtern<CImage, &CImage::link> unusedImages(0x0057EB68, 0x0057EB70);
const CListExtern<CSprite, &CSprite::link> unusedSprites(0x0063FE30, 0x0063FE34);

struct SpriteTileData {
  CSprite* tails[256];
  CSprite* heads[256];
};
SCBW_DATA(SpriteTileData*, spritesOnTileRow, 0x00629288);

//Player alliance status
struct PlayerAllianceData {
  u8 player[PLAYER_COUNT];
};
PlayerAllianceData* const playerAlliance = (PlayerAllianceData*) 0x0058D634;

//-------- Internal constants --------//

SCBW_DATA(const Bool32*,  IS_GAME_PAUSED,   0x006509C4);
SCBW_DATA(const Bool8*,   IS_BROOD_WAR,     0x0058F440);
SCBW_DATA(const u32*,     CHEAT_STATE,      0x006D5A6C);
SCBW_DATA(const u32*,     unitOrderingCount, 0x0066FF74);
SCBW_DATA(const s32*,     MAX_UNIT_WIDTH,   0x006BEE68);
SCBW_DATA(const s32*,     MAX_UNIT_HEIGHT,  0x006BB930);
SCBW_DATA(const Bool32*,  IS_IN_REPLAY,     0x006D0F14);
SCBW_DATA(const s32*,     LOCAL_NATION_ID,  0x00512684);  //AKA g_LocalNationID; Actually stores the player ID.
SCBW_DATA(const s32*,     LOCAL_HUMAN_ID,   0x00512688);  //AKA g_LocalHumanID; Invalid in replay games.
SCBW_DATA(const Bool32*,  IS_IN_GAME_LOOP,  0x006D11C8);
SCBW_DATA(u32*,           lastRandomNumber, 0x0051CA14);
SCBW_DATA(Bool32*,        canUpdatePoweredStatus, 0x0063FF44);

//-------- DAT Files --------//

// The following code is based on BWAPI's BW/Offsets.h
// you can get the other addresses in http://code.google.com/p/bwapi/source/browse/trunk/bwapi/BWAPI/Source/BW/Offsets.h
// Since StarCraft is already running when the plugin is loaded, the DatLoad
// structs are guaranteed to have the corrent offsets.

struct DatLoad {
  u32 address;
  u32 length;
  u32 entries;
};

namespace Unit {
const DatLoad* const unitsDat      = (const DatLoad*)(0x00513C30);

const u8* const   Graphic             = (const u8*)  unitsDat[0].address;
const u16* const  SubUnit             = (const u16*) unitsDat[1].address;
const u32* const  ConstructionGraphic = (const u32*) unitsDat[4].address;
const u8* const   ShieldsEnabled      = (const u8*)  unitsDat[6].address;
const u16* const  MaxShieldPoints     = (const u16*) unitsDat[7].address;
const s32* const  MaxHitPoints        = (const s32*) unitsDat[8].address;
const u8* const   Elevation           = (const u8*)  unitsDat[9].address;
const u8* const   GroundWeapon        = (const u8*)  unitsDat[17].address;
const u8* const   MaxGroundHits       = (const u8*)  unitsDat[18].address;
const u8* const   AirWeapon           = (const u8*)  unitsDat[19].address;
const u8* const   MaxAirHits          = (const u8*)  unitsDat[20].address;
const u32* const  BaseProperty        = (const u32*) unitsDat[22].address;  // AKA SpecialAbilityFlags, UnitProperty, etc. Use with UnitProperty::Enum.
const u8* const   SeekRange           = (const u8*)  unitsDat[23].address;  // AKA Target Acquisition Range, Acquire Target Range, etc.
const u8* const   SightRange          = (const u8*)  unitsDat[24].address;
const u8* const   ArmorUpgrade        = (const u8*)  unitsDat[25].address;
const u8* const   SizeType            = (const u8*)  unitsDat[26].address;
const u8* const   ArmorAmount         = (const u8*)  unitsDat[27].address;
const Rect16* const UnitBounds        = (const Rect16*) unitsDat[38].address;
const u16* const  MineralCost         = (const u16*) unitsDat[40].address;
const u16* const  GasCost             = (const u16*) unitsDat[41].address;
const u16* const  TimeCost            = (const u16*) unitsDat[42].address;
const GroupFlag* const GroupFlags     = (const GroupFlag*) unitsDat[44].address;
const u8* const   SupplyProvided      = (const u8*)  unitsDat[45].address;  // Supply shown in-game is (true supply) / 2; e.g. Zerglings have 1 true supply, Marines have 2 true supply, etc.
const u8* const   SupplyRequired      = (const u8*)  unitsDat[46].address;  // Supply shown in-game is (true supply) / 2; e.g. Zerglings have 1 true supply, Marines have 2 true supply, etc.
const u8* const   SpaceRequired       = (const u8*)  unitsDat[47].address;
const u8* const   SpaceProvided       = (const u8*)  unitsDat[48].address;
const u16* const  BuildScore          = (const u16*) unitsDat[49].address;
const u16* const  DestroyScore        = (const u16*) unitsDat[50].address;
const u16* const  MapStringId         = (const u16*) unitsDat[51].address;
const u8* const   BroodwarOnly        = (const u8*)  unitsDat[52].address;
}

namespace Flingy {
const DatLoad* const flingyDat    = (const DatLoad*)(0x00515A38);

const u16* const  SpriteID        = (const u16*) flingyDat[0].address;
const u32* const  TopSpeed        = (const u32*) flingyDat[1].address;
const u16* const  Acceleration    = (const u16*) flingyDat[2].address;
const u32* const  HaltDistance    = (const u32*) flingyDat[3].address;
const u8* const   TurnSpeed       = (const u8*)  flingyDat[4].address;  //Incorrectly known as "Turn Radius"
const u8* const   MovementControl = (const u8*)  flingyDat[6].address;
}

namespace Weapon {
const DatLoad* const weaponsDat     = (const DatLoad*)(0x00513868);

const u16* const Label              = (const u16*) weaponsDat[0].address;
const u32* const Graphic            = (const u32*) weaponsDat[1].address;
const u16* const TargetFlags        = (const u16*) weaponsDat[3].address;
const u32* const MinRange           = (const u32*) weaponsDat[4].address;
const u32* const MaxRange           = (const u32*) weaponsDat[5].address;
const u8* const  DamageUpgrade      = (const u8*)  weaponsDat[6].address;
const u8* const  DamageType         = (const u8*)  weaponsDat[7].address; //Normal, Explosive, etc.. Use with DamageType::Enum.
const u8* const  Behavior           = (const u8*)  weaponsDat[8].address; //Fly and follow target, appear on target unit, etc. Use with WeaponBehavior::Enum.
const u8* const  RemoveAfter        = (const u8*)  weaponsDat[9].address;
const u8* const  ExplosionType      = (const u8*)  weaponsDat[10].address;  //NormalHit, RadialSplash, EnemySplash, etc. Use with WeaponEffect::Enum.
const u16* const InnerSplashRadius  = (const u16*) weaponsDat[11].address;
const u16* const MediumSplashRadius = (const u16*) weaponsDat[12].address;
const u16* const OuterSplashRadius  = (const u16*) weaponsDat[13].address;
const u16* const DamageAmount       = (const u16*) weaponsDat[14].address;
const u16* const DamageBonus        = (const u16*) weaponsDat[15].address;
const u8* const  Cooldown           = (const u8*)  weaponsDat[16].address;
const u8* const  DamageFactor       = (const u8*)  weaponsDat[17].address;
const u8* const  AttackDirection    = (const u8*)  weaponsDat[18].address;
const u8* const  LaunchSpin         = (const u8*)  weaponsDat[19].address;
const u8* const  Xoffset            = (const u8*)  weaponsDat[20].address;
const u8* const  Yoffset            = (const u8*)  weaponsDat[21].address;
}

namespace Upgrade {
const DatLoad* const upgradesDat    = (const DatLoad*)(0x005136E0);

const u16* const  MineralCostBase   = (const u16*)  upgradesDat[0].address;
const u16* const  MineralCostFactor = (const u16*)  upgradesDat[1].address;
const u16* const  GasCostBase       = (const u16*)  upgradesDat[2].address;
const u16* const  GasCostFactor     = (const u16*)  upgradesDat[3].address;
const u16* const  TimeCostBase      = (const u16*)  upgradesDat[4].address;
const u16* const  TimeCostFactor    = (const u16*)  upgradesDat[5].address;
const u16* const  Label             = (const u16*)  upgradesDat[8].address;
const u8* const   Race              = (const u8*)   upgradesDat[9].address;
const u8* const   MaxRepeats        = (const u8*)   upgradesDat[10].address;
}

namespace Tech {
const DatLoad* const techdataDat  = (const DatLoad*)(0x005137D8);

const u16* const MineralCost      = (const u16*) techdataDat[0].address;
const u16* const GasCost          = (const u16*) techdataDat[1].address;
const u16* const TimeCost         = (const u16*) techdataDat[2].address;
const u16* const EnergyCost       = (const u16*) techdataDat[3].address;
const u16* const Label            = (const u16*) techdataDat[7].address;
}


//-------- Tech levels --------//

namespace Tech {
struct _scTechs { u8 enabled[PLAYER_COUNT][24]; };
struct _bwTechs { u8 enabled[PLAYER_COUNT][20]; };
_scTechs* const AvailableSc   = (_scTechs*) 0x0058CE24; //Use with ScTech::Enum
_scTechs* const ResearchedSc  = (_scTechs*) 0x0058CF44; //Use with ScTech::Enum
_bwTechs* const AvailableBw   = (_bwTechs*) 0x0058F050; //Use with BwTech::Enum
_bwTechs* const ResearchedBw  = (_bwTechs*) 0x0058F140; //Use with BwTech::Enum
}

//-------- Upgrade levels --------//

namespace Upgrade {
struct _scUpgrs { u8 level[PLAYER_COUNT][46];  };
struct _bwUpgrs { u8 level[PLAYER_COUNT][15];  };
static _scUpgrs* MaximumUpgSc = (_scUpgrs*) 0x0058D088; //Use with ScUpgrade::Enum
static _scUpgrs* CurrentUpgSc = (_scUpgrs*) 0x0058D2B0; //Use with ScUpgrade::Enum
static _bwUpgrs* MaximumUpgBw = (_bwUpgrs*) 0x0058F24A; //Use with BwUpgrade::Enum
static _bwUpgrs* CurrentUpgBw = (_bwUpgrs*) 0x0058F32C; //Use with BwUpgrade::Enum
}

#undef SCBW_DATA
#pragma pack()
