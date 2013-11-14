#include "flags.h"

namespace datcc {

//-------- units.dat flags --------//

extern const FlagNames<BYTE> unitMovementFlags = {
  "UnitCanMove_1",
  "Unused_2",
  "IsAir_4",
  "Unused_8",
  "Unused_10",
  "Unused_20",
  "IsAir_40",
  "IsHover_80"
};

extern const FlagNames<DWORD> unitPrototypeFlags = {
  "Building",
  "Addon",
  "Flyer",
  "Worker",
  "Subunit",
  "FlyingBuilding",
  "Hero",
  "RegeneratesHP",
  "AnimatedIdle",
  "Cloakable",
  "TwoUnitsIn1Egg",
  "SingleEntity",
  "ResourceDepot",
  "ResourceContainer",
  "RoboticUnit",
  "Detector",
  "Organic",
  "RequiresCreep",
  "UnusedFlag",
  "RequiredPsi",
  "Burrowable",
  "Spellcaster",
  "PermanentCloak",
  "PickupItem",
  "MorphFromOtherUnit",
  "MediumOverlay",
  "LargeOverlay",
  "AutoAttackAndMove",
  "CanAttack",
  "Invincible",
  "Mechanical",
  "ProducesUnits",
};

extern const FlagNames<BYTE> unitGroupFlags = {
  "Zerg",
  "Terran",
  "Protoss",
  "Men",
  "Building",
  "Factory",
  "Independent",
  "Neutral"
};

extern const FlagNames<WORD> unitAvailabilityFlags = {
  "NonNeutral",
  "UnitListing",
  "MissionBriefing",
  "PlayerSettings",
  "AllRaces",
  "SetDoodadState",
  "NonLocationTriggers",
  "UnitSettings",
  "LocationTriggers",
  "BroodwarOnly",
  "Unused_0x400",
  "Unused_0x800",
  "Unused_0x1000",
  "Unused_0x2000",
  "Unused_0x4000",
  "Unused_0x8000"
};

//-------- weapons.dat flags --------//

extern const FlagNames<WORD>  weaponTargetFlags = {
  "Air",
  "Ground",
  "Mechanical",
  "Organic",
  "NonBuilding",
  "NonRobotic",
  "Terrain",
  "OrganicOrMech",
  "PlayerOwn",
  "Unused_0x200",
  "Unused_0x400",
  "Unused_0x800",
  "Unused_0x1000",
  "Unused_0x2000",
  "Unused_0x4000",
  "Unused_0x8000"
};

//-------- Miscellaneous --------//

extern const FlagNames<BYTE>  unknownFlags8 = {
  "Unknown_0x1",
  "Unknown_0x2",
  "Unknown_0x4",
  "Unknown_0x8",
  "Unknown_0x10"
  "Unknown_0x20",
  "Unknown_0x40",
  "Unknown_0x80",
};

} //datcc
