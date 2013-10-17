// Contains wrappers for various SCBW internal functions, to be used in
// hook functions.

#pragma once
#include "scbwdata.h"
#include "enumerations.h"

namespace scbw {
/// Prints text to the screen, optionally using the given text color.
void printText(const char* text, u32 color = GameTextColor::White);

/// Plays the given sound from sfxdata.dat, optionally using a unit as the
/// source of the sound.
void playSound(u32 sfxId, const CUnit *sourceUnit = NULL);

/// Displays an error message and plays a sound for the player.
///
/// @param  playerId  ID of the player who receives the message.
/// @param  statTxtId ID of the error message string in stat_txt.tbl
/// @param  sfxId     ID of the sound file in sfxdata.dat.
void showErrorMessageWithSfx(u32 playerId, u32 statTxtId, u32 sfxId);

/// Retrieves a string from stat_txt.tbl, using the given @p index.
/// If an invalid @p index is specified, returns "" instead.
/// If @p index == 0, returns NULL.
/// Note: TBL string indexes start at 1, not 0.
const char* getStatTxtTblString(u16 index);

/// Checks whether the unit has an image overlay that matches the given ID.
bool hasOverlay(const CUnit* const unit, const u32 imageId);

/// Calculates the images.dat overlay adjustment for the given unit.
/// This piece of code is used throughout StarCraft.exe for various status
/// effects, such as Ensnare, Plague, and Defensive Matrix.
u32 getUnitOverlayAdjustment(const CUnit* const unit);

/// Makes the unit use the specified weapon to attack its current target unit in
/// the CUnit::orderTarget member. This does not affect the unit's weapon
/// cooldown. The spawned weapon sprite obeys the weapon behavior properties in
/// weapons.dat.
///
/// @param  unit        The unit that uses the weapon. If NULL, the function
///                     returns without error.
/// @param  weaponId    ID of the weapon in weapons.dat. If same or bigger than
///                     WEAPON_TYPE_COUNT, the function returns without error.
void fireUnitWeapon(CUnit* unit, u8 weaponId);

/// Attempts to create a unit at the specified coordinates. If the unit cannot
/// be created (e.g. there is no space), this function displays an error message
/// and returns NULL instead. This function is the same one used for creating
/// pre-placed units in UMS maps.
///
/// @return           The created unit or NULL if the unit cannot be created.
CUnit* createUnitAtPos(u16 unitType, u16 playerId, u32 x, u32 y);

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

/// StarCraft's internal function used to quickly calculate distances between
/// two points (x1, y1) and (x2, y2).
/// Warning: This function is inaccurate for long distances.
u32 getDistanceFast(s32 x1, s32 y1, s32 x2, s32 y2);

/// Returns the player's upgrade level of [upgradeId]. Note that this uses
/// UpgradeId::Enum, instead of ScUpgrades::Enum and BwUpgrades::Enum.
u8 getUpgradeLevel(const u8 playerId, const u8 upgradeId);

/// Returns the elevation of the tile at (x, y). 0 for low terrain, 1 for
/// medium, and 2 for high terrain.
u32 getGroundHeightAtPos(s32 x, s32 y);

/// Returns the pointer to the active tile data at (x, y).
inline ActiveTile& getActiveTileAt(s32 x, s32 y) {
  return (*activeTileArray)[(x / 32) + mapTileSize->width * (y / 32)];
}

/// Checks whether the given cheat flag is enabled.
inline bool isCheatEnabled(u32 cheatFlag) {
  return ((*CHEAT_STATE) & cheatFlag) != 0;
}

/// Checks whether the game is paused.
inline bool isGamePaused() {
  return (*IS_GAME_PAUSED) != 0;
}

/// Checks whether the game is in Brood War mode.
inline bool isBroodWarMode() {
  return (*IS_BROOD_WAR) != 0;
}

/// Checks whether @p playerB is recognized by @p playerA as an ally.
/// Warning: The opposite may not necessarily be true!
inline bool isAlliedTo(u8 playerA, u8 playerB) {
  return 0 != playerAlliance[playerA].flags[playerB];
}

/// Refreshes the screen position within the given rectangle area in pixels.
void refreshScreen(int left, int top, int right, int bottom);

/// Refreshes the entire screen.
void refreshScreen();

/// Something related to button sets, not completely understood.
void refreshButtonSet();

/// Generates a pseudorandom number between min and max, inclusive. This is
/// identical to the function used internally by StarCraft, and is guaranteed to
/// generate the same sequence in a replay.
/// NOTE: The RNG has a maximum range of 0 - 32767.
u32 randBetween(u32 min, u32 max);

//Sets the Anywhere location.
inline void setAnywhereLocation() {
  LOCATION* location = &locationTable[63];
  location->topLeftX = 0;
  location->topLeftY = 0;
  location->bottomRightX = mapTileSize->width * 32;
  location->bottomRightY = mapTileSize->height * 32;
  location->flags = 63;
}

inline void setLocation(int locNumber, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, int flags) {
  LOCATION* location = &locationTable[locNumber];
  location->topLeftX = topLeftX;
  location->topLeftY = topLeftY;
  location->bottomRightX = bottomRightX;
  location->bottomRightY = bottomRightY;
  location->flags = flags;
}

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

} //scbw
