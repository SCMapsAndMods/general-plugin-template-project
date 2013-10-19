// Contains the definitions of various data structures, arrays, pointers and
// internal constants in StarCraft.exe.
// The contents of this file are merged from: unit.cpp, triggers.cpp,
// resources.cpp, player.cpp, locations.cpp, buttons.cpp, and qdp.cpp.

#pragma once
#include "../definitions.h"
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
SCBW_DATA(CUnit**,      firstVisibleUnit,       0x00628430);
/// Includes units in transports/bunkers, workers inside gas buildings, nukes in
/// silos, queens inside command centers, and units in production.
SCBW_DATA(CUnit**,      firstHiddenUnit,        0x006283EC);
SCBW_DATA(CUnit**,      firstScannerSweep,      0x006283F4);
SCBW_DATA(CUnit**,      firstPsiFieldProvider,  0x0063FF54);
SCBW_DATA(CBullet**,    firstBullet,            0x0064DEC4);

struct Units12 { CUnit* unit[PLAYER_COUNT]; };
SCBW_DATA(Units12*,     firstPlayerUnit,        0x006283F8);  //Indexed by player Id

// Units that are selected by the current player (or the player viewing the replay).
SCBW_DATA(const Units12*, clientSelectionGroup, 0x00597208);
SCBW_DATA(const u8*,    clientSelectionCount,   0x0059723D);

//Contains various info on the tiles.
SCBW_DATA(ActiveTile**, activeTileArray,        0x006D1260);

// Stores an ordered list of all units currently in the game.
SCBW_DATA(UnitFinderData*, unitOrderingX,       0x0066FF78);
SCBW_DATA(UnitFinderData*, unitOrderingY,       0x006769B8);
SCBW_DATA(const u32*,   unitOrderingCount,      0x0066FF74);

// Font & Drawing
namespace graphics { class Font; class Bitmap; }
SCBW_DATA(graphics::Font**, fontBase,           0x006CE0F4);
SCBW_DATA(graphics::Bitmap*, gameScreenBuffer,  0x006CEFF0);
SCBW_DATA(u8*,          refreshRegions,         0x006CEFF8);  //640 x 480 divided into 1200 squares of 16x16
SCBW_DATA(s32*,         screenX,                0x00628448);
SCBW_DATA(s32*,         screenY,                0x00628470);
SCBW_DATA(const Point32*, mouse,                0x006CDDC4);
SCBW_DATA(Layers*,      screenLayers,           0x006CEF50);
typedef void (__stdcall *DrawGameProc)(graphics::Bitmap *surface, Bounds *bounds);
static DrawGameProc const oldDrawGameProc = (DrawGameProc) 0x004BD580;

SCBW_DATA(CImage**,     firstUnusedImage,       0x0057EB68);
SCBW_DATA(CImage**,     lastUnusedImage,        0x0057EB70);
const CListExtern<CImage, &CImage::link> unusedImages(0x0057EB68, 0x0057EB70);
const CListExtern<CSprite, &CSprite::link> unusedSprites(0x0063FE30, 0x0063FE34);

struct SpriteTileData {
  CSprite* tails[256];
  CSprite* heads[256];
};
SCBW_DATA(SpriteTileData*, spritesOnTileRow,    0x00629288);

template <typename T>
struct PlayerFlags {
  T flags[PLAYER_COUNT];
};
SCBW_DATA(PlayerFlags<u8>*, playerAlliance,     0x0058D634);
SCBW_DATA(PlayerFlags<u32>*, playerVision,      0x0057F1EC);

SCBW_DATA(u16* const*,    statTxtTbl,           0x006D1238);
SCBW_DATA(CUnit* const*,  activePortraitUnit,   0x00597248);

//-------- Internal constants --------//

SCBW_DATA(const Bool32*,  IS_GAME_PAUSED,       0x006509C4);
SCBW_DATA(const Bool8*,   IS_BROOD_WAR,         0x0058F440);
SCBW_DATA(const u32*,     CHEAT_STATE,          0x006D5A6C);
SCBW_DATA(const s32*,     MAX_UNIT_WIDTH,       0x006BEE68);
SCBW_DATA(const s32*,     MAX_UNIT_HEIGHT,      0x006BB930);
SCBW_DATA(const Bool32*,  IS_IN_REPLAY,         0x006D0F14);
SCBW_DATA(const s32*,     ACTIVE_NATION_ID,     0x00512678);  //AKA g_ActiveNationID
SCBW_DATA(const s32*,     LOCAL_NATION_ID,      0x00512684);  //AKA g_LocalNationID; Actually stores the player ID.
SCBW_DATA(const s32*,     LOCAL_HUMAN_ID,       0x00512688);  //AKA g_LocalHumanID; Invalid in replay games.
SCBW_DATA(const Bool32*,  IS_IN_GAME_LOOP,      0x006D11C8);
SCBW_DATA(u32*,           lastRandomNumber,     0x0051CA14);
SCBW_DATA(Bool32*,        canUpdatePoweredStatus, 0x0063FF44);
SCBW_DATA(const Bool32*,  IS_PLACING_BUILDING,  0x00640880);

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
SCBW_DATA(const DatLoad*, unitsDat, 0x00513C30);

SCBW_DATA(u8*,  Graphic,            unitsDat[0].address);
SCBW_DATA(u16*, SubUnit,            unitsDat[1].address);
SCBW_DATA(u32*, ConstructionGraphic,  unitsDat[4].address);
SCBW_DATA(u8*,  ShieldsEnabled,     unitsDat[6].address);
SCBW_DATA(u16*, MaxShieldPoints,    unitsDat[7].address);
SCBW_DATA(s32*, MaxHitPoints,       unitsDat[8].address);
SCBW_DATA(u8*,  Elevation,          unitsDat[9].address);
SCBW_DATA(u8*,  MovementFlags,      unitsDat[10].address);
SCBW_DATA(u8*,  GroundWeapon,       unitsDat[17].address);
SCBW_DATA(u8*,  MaxGroundHits,      unitsDat[18].address);
SCBW_DATA(u8*,  AirWeapon,          unitsDat[19].address);
SCBW_DATA(u8*,  MaxAirHits,         unitsDat[20].address);
SCBW_DATA(u32*, BaseProperty,       unitsDat[22].address);  // AKA SpecialAbilityFlags, UnitProperty, etc. Use with UnitProperty::Enum.
SCBW_DATA(u8*,  SeekRange,          unitsDat[23].address);  // AKA Target Acquisition Range, Acquire Target Range, etc.
SCBW_DATA(u8*,  SightRange,         unitsDat[24].address);
SCBW_DATA(u8*,  ArmorUpgrade,       unitsDat[25].address);
SCBW_DATA(u8*,  SizeType,           unitsDat[26].address);
SCBW_DATA(u8*,  ArmorAmount,        unitsDat[27].address);
SCBW_DATA(Box16*, UnitBounds,       unitsDat[38].address);
SCBW_DATA(u16*, MineralCost,        unitsDat[40].address);
SCBW_DATA(u16*, GasCost,            unitsDat[41].address);
SCBW_DATA(u16*, TimeCost,           unitsDat[42].address);
SCBW_DATA(GroupFlag*, GroupFlags,   unitsDat[44].address);
SCBW_DATA(u8*,  SupplyProvided,     unitsDat[45].address);  // Supply shown in-game is (true supply) / 2; e.g. Zerglings have 1 true supply, Marines have 2 true supply, etc.
SCBW_DATA(u8*,  SupplyRequired,     unitsDat[46].address);  // Supply shown in-game is (true supply) / 2; e.g. Zerglings have 1 true supply, Marines have 2 true supply, etc.
SCBW_DATA(u8*,  SpaceRequired,      unitsDat[47].address);
SCBW_DATA(u8*,  SpaceProvided,      unitsDat[48].address);
SCBW_DATA(u16*, BuildScore,         unitsDat[49].address);
SCBW_DATA(u16*, DestroyScore,       unitsDat[50].address);
SCBW_DATA(u16*, MapStringId,        unitsDat[51].address);
SCBW_DATA(u8*,  BroodwarOnly,       unitsDat[52].address);
}

namespace Flingy {
SCBW_DATA(const DatLoad*, flingyDat, 0x00515A38);

SCBW_DATA(u16*, SpriteID,           flingyDat[0].address);
SCBW_DATA(u32*, TopSpeed,           flingyDat[1].address);
SCBW_DATA(u16*, Acceleration,       flingyDat[2].address);
SCBW_DATA(u32*, HaltDistance,       flingyDat[3].address);
SCBW_DATA(u8*,  TurnSpeed,          flingyDat[4].address);  //Incorrectly known as "Turn Radius"
SCBW_DATA(u8*,  MovementControl,    flingyDat[6].address);
}

namespace Weapon {
SCBW_DATA(const DatLoad*, weaponsDat, 0x00513868);

SCBW_DATA(u16*, Label,              weaponsDat[0].address);
SCBW_DATA(u32*, Graphic,            weaponsDat[1].address);
SCBW_DATA(TargetFlag*, TargetFlags, weaponsDat[3].address);
SCBW_DATA(u32*, MinRange,           weaponsDat[4].address);
SCBW_DATA(u32*, MaxRange,           weaponsDat[5].address);
SCBW_DATA(u8*,  DamageUpgrade,      weaponsDat[6].address);
SCBW_DATA(u8*,  DamageType,         weaponsDat[7].address); //Normal, Explosive, etc.. Use with DamageType::Enum.
SCBW_DATA(u8*,  Behavior,           weaponsDat[8].address); //Fly and follow target, appear on target unit, etc. Use with WeaponBehavior::Enum.
SCBW_DATA(u8*,  RemoveAfter,        weaponsDat[9].address);
SCBW_DATA(u8*,  ExplosionType,      weaponsDat[10].address);  //NormalHit, RadialSplash, EnemySplash, etc. Use with WeaponEffect::Enum.
SCBW_DATA(u16*, InnerSplashRadius,  weaponsDat[11].address);
SCBW_DATA(u16*, MediumSplashRadius, weaponsDat[12].address);
SCBW_DATA(u16*, OuterSplashRadius,  weaponsDat[13].address);
SCBW_DATA(u16*, DamageAmount,       weaponsDat[14].address);
SCBW_DATA(u16*, DamageBonus,        weaponsDat[15].address);
SCBW_DATA(u8*,  Cooldown,           weaponsDat[16].address);
SCBW_DATA(u8*,  DamageFactor,       weaponsDat[17].address);
SCBW_DATA(u8*,  AttackDirection,    weaponsDat[18].address);
SCBW_DATA(u8*,  LaunchSpin,         weaponsDat[19].address);
SCBW_DATA(u8*,  Xoffset,            weaponsDat[20].address);
SCBW_DATA(u8*,  Yoffset,            weaponsDat[21].address);
}

namespace Upgrade {
SCBW_DATA(const DatLoad*, upgradesDat, 0x005136E0);

SCBW_DATA(u16*, MineralCostBase,    upgradesDat[0].address);
SCBW_DATA(u16*, MineralCostFactor,  upgradesDat[1].address);
SCBW_DATA(u16*, GasCostBase,        upgradesDat[2].address);
SCBW_DATA(u16*, GasCostFactor,      upgradesDat[3].address);
SCBW_DATA(u16*, TimeCostBase,       upgradesDat[4].address);
SCBW_DATA(u16*, TimeCostFactor,     upgradesDat[5].address);
SCBW_DATA(u16*, Label,              upgradesDat[8].address);
SCBW_DATA(u8*,  Race,               upgradesDat[9].address);
SCBW_DATA(u8*,  MaxRepeats,         upgradesDat[10].address);
}

namespace Tech {
SCBW_DATA(const DatLoad*, techdataDat, 0x005137D8);

SCBW_DATA(u16*, MineralCost,        techdataDat[0].address);
SCBW_DATA(u16*, GasCost,            techdataDat[1].address);
SCBW_DATA(u16*, TimeCost,           techdataDat[2].address);
SCBW_DATA(u16*, EnergyCost,         techdataDat[3].address);
SCBW_DATA(u16*, Label,              techdataDat[7].address);
}


//-------- Tech levels --------//

namespace Tech {
struct _scTechs { u8 enabled[PLAYER_COUNT][24]; };
struct _bwTechs { u8 enabled[PLAYER_COUNT][20]; };
SCBW_DATA(_scTechs*, AvailableSc,   0x0058CE24); //Use with ScTech::Enum
SCBW_DATA(_scTechs*, ResearchedSc,  0x0058CF44); //Use with ScTech::Enum
SCBW_DATA(_bwTechs*, AvailableBw,   0x0058F050); //Use with BwTech::Enum
SCBW_DATA(_bwTechs*, ResearchedBw,  0x0058F140); //Use with BwTech::Enum
}

//-------- Upgrade levels --------//

namespace Upgrade {
struct _scUpgrs { u8 level[PLAYER_COUNT][46];  };
struct _bwUpgrs { u8 level[PLAYER_COUNT][15];  };
SCBW_DATA(_scUpgrs*, MaximumUpgSc,  0x0058D088); //Use with ScUpgrade::Enum
SCBW_DATA(_scUpgrs*, CurrentUpgSc,  0x0058D2B0); //Use with ScUpgrade::Enum
SCBW_DATA(_bwUpgrs*, MaximumUpgBw,  0x0058F24A); //Use with BwUpgrade::Enum
SCBW_DATA(_bwUpgrs*, CurrentUpgBw,  0x0058F32C); //Use with BwUpgrade::Enum
}

#undef SCBW_DATA
#pragma pack()
