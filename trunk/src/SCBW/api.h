// Contains wrappers for various SCBW internal functions, to be used in
// hook functions.

#pragma once
#include "scbwdata.h"
#include "enumerations.h"

namespace scbw {
/// Prints text to the screen, optionally using the given text color.
void printText(char* text, DWORD color = GameTextColor::White);

/// Plays the given sound from sfxdata.dat, optionally using a unit as the
/// source of the sound.
void playSound(u32 sfxId, const CUnit *sourceUnit = NULL);

/// Deals damage to the target unit, as though it was attacked by another unit.
void doWeaponDamage(DWORD   damage,
                    CUnit*  target,
                    BYTE    weaponId,             ///< Used to determine damage type (normal, concussive, explosive, etc.)
                    CUnit*  attacker = NULL,      ///< This unit receives kill count if the target dies
                    DWORD   attackingPlayer = -1, ///< For increasing kill scores and checking whether to apply the Power Overwhelming cheat
                    BYTE    direction = 0,        ///< Used to create plasma shield flickering overlays
                    DWORD   dmgDivisor = 1);      ///< Used for calculating splash damage or Glave Wurm bounce damage.

/// Creates an image overlay with the specified images.dat ID on the given sprite.
/// Usage:  CreateOverlay(unit->sprite, 105);
void createOverlay(CSprite* sprite, DWORD imageId, BYTE x = 0, BYTE y = 0, DWORD direction = 0);

/// Loops through the unit's image overlays, removing any that matches the given images.dat IDs.
void removeOverlays(CUnit *unit, DWORD imageIdStart, DWORD imageIdEnd);

/// Loops through the unit's image overlays, removing any that matches the given images.dat ID.
inline void removeOverlays(CUnit *unit, DWORD imageId) {
  removeOverlays(unit, imageId, imageId);
}

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

/// Makes the given image play the specified Iscript animation entry.
///
/// @param  image       The image to play the animation. If NULL, the function
///                     returns without error.
/// @param  animation   ID of the Iscript animation to play. See
///                     IscriptAnimation::Enum for a list of animations.
void playIscriptAnim(CImage* image, u8 animation);

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
  return (*activeTileArray)[(x / 32) + mapSize->width * (y / 32)];
}

/// Checks whether the given cheat flag is enabled.
inline bool isCheatEnabled(u32 cheatFlag) {
  return ((*CHEAT_STATE) & cheatFlag) != 0;
}

/// Checks whether the game is in Brood War mode.
inline bool isBroodWarMode() {
  return (*IS_BROOD_WAR) != 0;
}

/// Checks whether @p playerB is recognized by @p playerA as an ally.
/// Warning: The opposite may not necessarily be true!
inline bool isAlliedTo(u8 playerA, u8 playerB) {
  return 0 != playerAlliance[playerA].player[playerB];
}

/// Populates @p unitsFound with all units within the given bounds. This is much
/// more efficient than manually looping through the entire unit table.
///
/// @param [out]  unitsFound  An array of CUnit pointers that would be populated
///                           with the units found. The size of this array
///                           should be 1700 (UNIT_ARRAY_LENGTH).
/// @return                   The number of units found.
int findUnitsInBounds(s32 left, s32 right, s32 top, s32 bottom,
                         CUnit** unitsFound);

/// Iterates through all units within the given bounds until it finds a unit for
/// which @p isGood() returns true.
///
/// @param isGood   A callback function or functor that accepts a Unit pointer
///                 as its only argument, and returns a boolean value that
///                 indicates whether the search should be stopped.
/// @return         The unit for which isGood() returns true. If no unit passes
///                 the test, returns NULL.
template <typename callbackT>
CUnit* findFirstUnitInBounds(s32 left, s32 right, s32 top, s32 bottom,
                             callbackT &isGood) {
  CUnit* unitsFound[UNIT_ARRAY_LENGTH];
  int unitsFoundCount = findUnitsInBounds(left, right, top, bottom, unitsFound);

  for (int i = 0; i < unitsFoundCount; ++i) {
    CUnit *unit = unitsFound[i];
    if (isGood(unit))
      return unit;
  }

  return NULL;
}

/// Iterates through all units within the given bounds, and returns the unit for
/// which @ checkScore() returns the highest value.
///
/// @param  checkScore  A callback function or functor that accepts a Unit
///                     pointer as its only argument, and returns the relative
///                     score (unsigned int) for that unit.
/// @return             The unit that receives the highest score value. If
///                     @p checkScore() returns 0 for all units, returns NULL.
template <typename callbackT>
CUnit* findBestUnitInBounds(s32 left, s32 right, s32 top, s32 bottom,
                            callbackT &checkScore) {
  CUnit* unitsFound[UNIT_ARRAY_LENGTH];
  int unitsFoundCount = findUnitsInBounds(left, right, top, bottom, unitsFound);

  unsigned int bestScore = 0;
  CUnit *bestUnit = NULL;

  for (int i = 0; i < unitsFoundCount; ++i) {
    CUnit *unit = unitsFound[i];
    unsigned int currentScore = checkScore(unit);
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestUnit = unit;
    }
  }

  return bestUnit;
}

} //scbw
