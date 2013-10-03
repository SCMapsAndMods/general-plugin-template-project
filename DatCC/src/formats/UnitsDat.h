#pragma once
#include "DatInterface.h"
#include <sstream>

namespace datcc {

const int UNIT_TYPE_COUNT = 228;
const int UNITS_DAT_UNITS = 106;  //Unit IDs 0-105
const int UNITS_DAT_BLDGS = 96;   //Unit IDs 106-201

//Based on http://modcrafters.com/wiki/index.php?title=Units.dat

struct UnitsDatFile {
  BYTE  flingy          [UNIT_TYPE_COUNT];
  WORD  subUnit1        [UNIT_TYPE_COUNT];
  WORD  subUnit2        [UNIT_TYPE_COUNT];
  WORD  infestChangeUnit[UNITS_DAT_BLDGS];
  DWORD constructionAnimation[UNIT_TYPE_COUNT];
  BYTE  spawnDirection  [UNIT_TYPE_COUNT];
  BYTE  hasShields      [UNIT_TYPE_COUNT];
  WORD  maxShields      [UNIT_TYPE_COUNT];
  DWORD maxHitPoints    [UNIT_TYPE_COUNT];
  BYTE  elevationLevel  [UNIT_TYPE_COUNT];
  BYTE  movementFlags   [UNIT_TYPE_COUNT];
  BYTE  rank            [UNIT_TYPE_COUNT];
  BYTE  computerAiIdleOrder[UNIT_TYPE_COUNT];
  BYTE  humanAiIdleOrder[UNIT_TYPE_COUNT];
  BYTE  returnToIdleOrder[UNIT_TYPE_COUNT];
  BYTE  attackUnitOrder [UNIT_TYPE_COUNT];
  BYTE  attackMoveOrder [UNIT_TYPE_COUNT];
  BYTE  groundWeapon    [UNIT_TYPE_COUNT];
  BYTE  maxGroundHits   [UNIT_TYPE_COUNT];
  BYTE  airWeapon       [UNIT_TYPE_COUNT];
  BYTE  maxAirHits      [UNIT_TYPE_COUNT];
  BYTE  aiInternalFlags [UNIT_TYPE_COUNT];
  DWORD prototypeFlags  [UNIT_TYPE_COUNT];
  BYTE  seekRange       [UNIT_TYPE_COUNT];
  BYTE  sightRange      [UNIT_TYPE_COUNT];
  BYTE  armorUpgrade    [UNIT_TYPE_COUNT];
  BYTE  unitSize        [UNIT_TYPE_COUNT];
  BYTE  armor           [UNIT_TYPE_COUNT];
  BYTE  rightClickAction[UNIT_TYPE_COUNT];
  WORD  readySfx        [UNITS_DAT_UNITS];
  WORD  whatFirstSfx    [UNIT_TYPE_COUNT];
  WORD  whatLastSfx     [UNIT_TYPE_COUNT];
  WORD  pissedFirstSfx  [UNITS_DAT_UNITS];
  WORD  pissedLastSfx   [UNITS_DAT_UNITS];
  WORD  yesFirstSfx     [UNITS_DAT_UNITS];
  WORD  yesLastSfx      [UNITS_DAT_UNITS];
  WORD  unitWidth       [UNIT_TYPE_COUNT];
  WORD  unitHeight      [UNIT_TYPE_COUNT];
  SWORD addonOffsetX    [UNITS_DAT_BLDGS];
  SWORD addonOffsetY    [UNITS_DAT_BLDGS];
  SWORD unitBoxLeft     [UNIT_TYPE_COUNT];
  SWORD unitBoxTop      [UNIT_TYPE_COUNT];
  SWORD unitBoxRight    [UNIT_TYPE_COUNT];
  SWORD unitBoxBottom   [UNIT_TYPE_COUNT];
  WORD  portrait        [UNIT_TYPE_COUNT];
  WORD  mineralCost     [UNIT_TYPE_COUNT];
  WORD  gasCost         [UNIT_TYPE_COUNT];
  WORD  buildTime       [UNIT_TYPE_COUNT];
  WORD  unknown         [UNIT_TYPE_COUNT];
  BYTE  groupFlags      [UNIT_TYPE_COUNT];
  BYTE  supplyProvided  [UNIT_TYPE_COUNT];
  BYTE  supplyCost      [UNIT_TYPE_COUNT];
  BYTE  cargoSize       [UNIT_TYPE_COUNT];
  BYTE  cargoSpace      [UNIT_TYPE_COUNT];
  WORD  buildScore      [UNIT_TYPE_COUNT];
  WORD  destroyScore    [UNIT_TYPE_COUNT];
  WORD  mapString       [UNIT_TYPE_COUNT];
  BYTE  isBroodWarUnit  [UNIT_TYPE_COUNT];
  WORD  availabilityFlags[UNIT_TYPE_COUNT];
};

C_ASSERT(sizeof(UnitsDatFile) == 19876);

class UnitsDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }
    
    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    UnitsDatFile data;
};

template <class IniProcT>
void UnitsDat::processIni(IniProcT &iniProc) {
  for (int unitId = 0; unitId < UNIT_TYPE_COUNT; ++unitId) {
    std::ostringstream sectionName;
    sectionName << "Unit #" << unitId;
    iniProc.setSection(sectionName.str(), makeUnitSectionComment(unitId));

    iniProc.processFlingyId(data.flingy[unitId],    "Flingy");
    iniProc.processUnitId(data.subUnit1[unitId],    "SubUnit 1");
    iniProc.processUnitId(data.subUnit2[unitId],    "SubUnit 2");
    iniProc.process(data.constructionAnimation[unitId], "Construction Animation");
    iniProc.process(data.spawnDirection[unitId],    "Spawn Direction");
    iniProc.process(data.hasShields[unitId],        "Has Shields");
    iniProc.process(data.maxShields[unitId],        "Max Shields");
    iniProc.process(data.maxHitPoints[unitId],      "Max HP");
    iniProc.process(data.unitSize[unitId],          "Unit Size");
    iniProc.process(data.armor[unitId],             "Armor");
    iniProc.processUpgradeId(data.armorUpgrade[unitId], "Armor Upgrade");

    iniProc.process(data.elevationLevel[unitId],    "Elevation Level");
    iniProc.processFlags(data.movementFlags[unitId], "Movement Flags");

    //AI-related
    iniProc.process(data.rank[unitId],              "Rank");
    iniProc.process(data.computerAiIdleOrder[unitId], "Comp AI Idle Order");
    iniProc.process(data.humanAiIdleOrder[unitId],  "Human AI Idle Order");
    iniProc.process(data.returnToIdleOrder[unitId], "Return Idle Order");
    iniProc.process(data.attackUnitOrder[unitId],   "Attack Unit Order");
    iniProc.process(data.attackMoveOrder[unitId],   "Attack Move Order");
    iniProc.process(data.seekRange[unitId],         "Seek Range");
    iniProc.process(data.sightRange[unitId],        "Sight Range");
    iniProc.process(data.rightClickAction[unitId],  "Right-Click Action");
    iniProc.process(data.aiInternalFlags[unitId],   "AI Internal Flags");

    iniProc.process(data.groundWeapon[unitId],      "Ground Weapon");
    iniProc.process(data.maxGroundHits[unitId],     "Ground Weapon Hits");
    iniProc.process(data.airWeapon[unitId],         "Air Weapon");
    iniProc.process(data.maxAirHits[unitId],        "Air Weapon Hits");
    iniProc.processFlags(data.prototypeFlags[unitId], "Prototype Flags");

    //Sounds
    iniProc.process(data.whatFirstSfx[unitId],      "What Sound (First)");
    iniProc.process(data.whatLastSfx[unitId],       "What Sound (Last)");
    //Process unit-specific data
    if (0 <= unitId && unitId <= 105) {
      iniProc.process(data.readySfx[unitId],        "Ready Sound");
      iniProc.process(data.pissedFirstSfx[unitId],  "Annoyed Sound (First)");
      iniProc.process(data.pissedLastSfx[unitId],   "Annoyed Sound (Last)");
      iniProc.process(data.yesFirstSfx[unitId],     "Yes Sound (First)");
      iniProc.process(data.yesLastSfx[unitId],      "Yes Sound (Last)");
    }

    iniProc.process(data.unitWidth[unitId],         "Unit Width");
    iniProc.process(data.unitHeight[unitId],        "Unit Height");
    iniProc.process(data.unitBoxLeft[unitId],       "Unit Box (Left)");
    iniProc.process(data.unitBoxTop[unitId],        "Unit Box (Top)");
    iniProc.process(data.unitBoxRight[unitId],      "Unit Box (Right)");
    iniProc.process(data.unitBoxBottom[unitId],     "Unit Box (Bottom)");
    iniProc.process(data.portrait[unitId],          "Portrait");
    iniProc.process(data.mineralCost[unitId],       "Mineral Cost");
    iniProc.process(data.gasCost[unitId],           "Gas Cost");
    iniProc.process(data.buildTime[unitId],         "Build Time");
    iniProc.process(data.unknown[unitId],           "Unknown");

    iniProc.processFlags(data.groupFlags[unitId],   "Group Flags");

    iniProc.process(data.supplyProvided[unitId],    "Supply Provided");
    iniProc.process(data.supplyCost[unitId],        "Supply Cost");
    iniProc.process(data.cargoSize[unitId],         "Cargo Size");
    iniProc.process(data.cargoSpace[unitId],        "Cargo Space");
    iniProc.process(data.buildScore[unitId],        "Build Score");
    iniProc.process(data.destroyScore[unitId],      "Destroy Score");
    iniProc.process(data.mapString[unitId],         "Map String");
    iniProc.process(data.isBroodWarUnit[unitId],    "Is Brood War Unit");

    iniProc.processFlags(data.availabilityFlags[unitId], "Availability Flags");
    
    //Process building-specific data
    if (106 <= unitId && unitId <= 201) {
      iniProc.processUnitId(data.infestChangeUnit[unitId - 106], "InfestChangeUnit");
      iniProc.process(data.addonOffsetX[unitId - 106], "Addon Offset (X)");
      iniProc.process(data.addonOffsetY[unitId - 106], "Addon Offset (Y)");
    }

  }
}


} //datcc
