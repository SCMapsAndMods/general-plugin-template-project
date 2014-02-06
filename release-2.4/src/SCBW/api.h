// Contains wrappers for various SCBW internal functions, to be used in
// hook functions.

#pragma once
#include <SCBW/scbwdata.h>
#include <SCBW/enumerations.h>

namespace scbw {

/// @name Output functions
//////////////////////////////////////////////////////////////// @{

/// Plays the given sound from sfxdata.dat. If @p sourceUnit is provided, it is
/// used as the source of the sound.
void playSound(u32 sfxId, const CUnit *sourceUnit = nullptr);

/// Prints text to the screen, optionally using the given text color.
void printText(const char* text, u32 color = GameTextColor::White);

/// Displays an error message and plays a sound for the player.
///
/// @param  playerId  ID of the player who receives the message.
/// @param  statTxtId ID of the error message string in stat_txt.tbl
/// @param  sfxId     ID of the sound file in sfxdata.dat.
void showErrorMessageWithSfx(u32 playerId, u32 statTxtId, u32 sfxId);

//////////////////////////////////////////////////////////////// @}

/// @name Unit checks
//////////////////////////////////////////////////////////////// @{

/// Checks whether the transport can hold this unit.
///
/// This function checks whether [transport] has any status effects (Lockdown,
/// Maelstrom, etc.), is owned by the same player, and has enough space for
/// [unit]. It does NOT check whether [transport] is a hallucination, or is an
/// Overlord without the transport upgrade, or whether the [unit] itself has any
/// status effects. Note that this function is affected by the EXE edits
/// "change bunker size check to organic flag check" and
/// "remove bunker size check" in FireGraft.
bool canBeEnteredBy(const CUnit* transport, const CUnit* unit);

/// Checks weapons.dat targeting flags to see if the @p target can be hit by the
/// @p weapon.
///
/// @param  weaponId  If this is an invalid weapons.dat ID, returns false.
/// @param  target    If nullptr is passed, the function checks whether the
///                   weapon can target a position on the ground. If @p target
///                   is invincible, the function returns false.
bool canWeaponTargetUnit(u8 weaponId, const CUnit *target = nullptr,
                         const CUnit *attacker = nullptr);

/// Checks if @p unit is under a Dark Swarm. This does NOT check whether the
/// unit is a ground unit or a building.
bool isUnderDarkSwarm(const CUnit *unit);

//////////////////////////////////////////////////////////////// @}

/// @name Graphics and geometry
//////////////////////////////////////////////////////////////// @{

/// Calculates the angle (in binary radians) of the arrow that starts at
/// (xTail, yTail) and ends at (xHead, yHead).
s32 getAngle(s32 xHead, s32 yHead, s32 xTail, s32 yTail);

/// StarCraft's internal function used to quickly calculate distances between
/// two points (x1, y1) and (x2, y2).
/// Warning: This function is inaccurate for long distances.
u32 getDistanceFast(s32 x1, s32 y1, s32 x2, s32 y2);

/// Calculates the X Cartesian coordinate from the given polar coordinates
/// (distance, angle), using StarCraft's internal data.
s32 getPolarX(s32 distance, u8 angle);

/// Calculates the Y Cartesian coordinate from the given polar coordinates
/// (distance, angle), using StarCraft's internal data.
s32 getPolarY(s32 distance, u8 angle);

//////////////////////////////////////////////////////////////// @}

/// @name Game information
//////////////////////////////////////////////////////////////// @{

/// Checks whether the game is in Brood War mode (instead of vanilla StarCraft).
inline bool isBroodWarMode() {
  return (*IS_BROOD_WAR) != 0;
}

/// Checks whether the given cheat flag is enabled.
inline bool isCheatEnabled(u32 cheatFlag) {
  return ((*CHEAT_STATE) & cheatFlag) != 0;
}

/// Checks whether the game is paused.
inline bool isGamePaused() {
  return (*IS_GAME_PAUSED) != 0;
}

/// Checks whether the game is in replay mode.
inline bool isInReplay() {
  return (*IS_IN_REPLAY) != 0;
}

//////////////////////////////////////////////////////////////// @}

/// @name Player information
//////////////////////////////////////////////////////////////// @{

/// Checks whether @p playerB is recognized by @p playerA as an ally.
/// Warning: The opposite may not necessarily be true!
inline bool isAlliedTo(u8 playerA, u8 playerB) {
  return 0 != playerAlliance[playerA].flags[playerB];
}

/// Returns the amount of remaining supply (total available - total used) for
/// the @p playerId, using @p raceId to determine the appropriate race to use.
/// This is affected by the "Food For Thought" cheat flag.
s32 getSupplyRemaining(u8 playerId, u8 raceId);

/// Checks whether the @p playerId has the @p techId researched.
/// Note: This uses TechId::Enum, instead of ScTech::Enum and BwTech::Enum.
bool hasTechResearched(u8 playerId, u16 techId);

/// Sets the player's tech research state for @p techId to @p isResearched.
/// Note: This uses TechId::Enum, instead of ScTech::Enum and BwTech::Enum.
void setTechResearchState(u8 playerId, u16 techId, bool isResearched);

/// Returns the player's upgrade level of @p upgradeId. Note that this uses
/// UpgradeId::Enum, instead of ScUpgrades::Enum and BwUpgrades::Enum.
u8 getUpgradeLevel(u8 playerId, u8 upgradeId);

/// Sets the player's upgrade level of @p upgradeId. Note that this uses
/// UpgradeId::Enum, instead of ScUpgrades::Enum and BwUpgrades::Enum.
void setUpgradeLevel(u8 playerId, u8 upgradeId, u8 level);

//////////////////////////////////////////////////////////////// @}

/// @name Map information
//////////////////////////////////////////////////////////////// @{

/// Returns the pointer to the active tile data at (x, y).
inline ActiveTile& getActiveTileAt(s32 x, s32 y) {
  return (*activeTileArray)[(x / 32) + mapTileSize->width * (y / 32)];
}

/// Returns the elevation of the tile at (x, y). 0 for low terrain, 1 for
/// medium, and 2 for high terrain.
u32 getGroundHeightAtPos(s32 x, s32 y);

//////////////////////////////////////////////////////////////// @}

/// @name Utility functions
//////////////////////////////////////////////////////////////// @{

/// Attempts to create a unit at the specified coordinates. If the unit cannot
/// be created (e.g. there is no space), this function displays an error message
/// and returns nullptr instead. This function is the same one used for creating
/// pre-placed units in UMS maps.
///
/// @return           The created unit or nullptr if the unit cannot be created.
CUnit* createUnitAtPos(u16 unitType, u16 playerId, u32 x, u32 y);

/// Calculates the images.dat overlay adjustment for the given unit.
/// This piece of code is used throughout StarCraft.exe for various status
/// effects, such as Ensnare, Plague, and Defensive Matrix.
u32 getUnitOverlayAdjustment(const CUnit* const unit);

/// Refreshes the screen position within the given rectangle area in pixels.
void refreshScreen(int left, int top, int right, int bottom);

/// Refreshes the entire screen.
void refreshScreen();

/// Generates a pseudorandom number between min and max, inclusive. This is
/// identical to the function used internally by StarCraft, and is guaranteed to
/// generate the same sequence in a replay.
/// NOTE: The RNG has a maximum range of 0 - 32767.
u32 randBetween(u32 min, u32 max);

/// Generates a pseudorandom number between 0 and 32767, inclusive. This is
/// identical to the function used internally by StarCraft, and is guaranteed to
/// generate the same sequence in a replay.
u16 random();

//////////////////////////////////////////////////////////////// @}

/// @name Needs research
//////////////////////////////////////////////////////////////// @{

/// Tells StarCraft to refresh the console (current button set, unit portrait,
/// and status bar). not completely understood.
void refreshConsole();

// Functionally identical to the [playfram] opcode in iscript.bin.
inline void playFrame(CImage* image, u16 frameset) {
  if (image->frameSet != frameset) {
    image->frameSet = frameset;
    u16 frameIndex = frameset + image->direction;
    if (image->frameIndex != frameIndex) {
      image->flags |= 0x1;	//Order the game to redraw the image
      image->frameIndex = frameIndex;
    }
  }
}

/// Sets the data of the Anywhere location.
inline void setAnywhereLocation() {
  LOCATION* location = &locationTable[63];
  location->dimensions.left   = 0;
  location->dimensions.top    = 0;
  location->dimensions.right  = 32 * mapTileSize->width;
  location->dimensions.bottom = 32 * mapTileSize->height;
  location->flags = 63;
}

/// Sets the data of location @p locNumber.
inline void setLocation(int locNumber, int left, int top, int right, int bottom, int flags) {
  LOCATION* location = &locationTable[locNumber];
  location->dimensions.left   = left;
  location->dimensions.top    = top;
  location->dimensions.right  = right;
  location->dimensions.bottom = bottom;
  location->flags = flags;
}

//////////////////////////////////////////////////////////////// @}

} //scbw
