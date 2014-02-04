//This header file contains member function declarations for CUnit.
//For the structure layout, see CUnitLayout.h

#pragma once
#include "CUnitLayout.h"
#include <SCBW/enumerations.h>

//Wrapper structure that provides methods for the CUnit structure.
struct CUnit: public CUnitLayout {
  //Returns the race of the current unit.
  //@return   RaceId::Terran, RaceId::Zerg, RaceId::Protoss, or RaceId::Neutral.
  RaceId::Enum getRace() const;

  //Returns the race of the unit with @p unitId.
  //@return   RaceId::Terran, RaceId::Zerg, RaceId::Protoss, or RaceId::Neutral.
  static RaceId::Enum getRace(u16 unitId);

  /// Sets the unit's HP, and also updates burning / bleeding graphics and unit
  /// strength data (used by the AI). HP is guaranteed not to overflow.
  void setHp(s32 hitPoints);

  /// Deals damage to this unit, using a specific weapons.dat ID.
  void damageWith(s32 damage,               ///< Amount of damage dealt to this unit.
                  u8 weaponId,              ///< weapons.dat ID to use.
                  CUnit *attacker = NULL,   ///< Attacking unit (for increasing kill count)
                  s8 attackingPlayer = -1,  ///< Attacking player (for increasing kill score)
                  s8 direction = 0,         ///< Attacked direction (for shield flicker overlays)
                  u8 damageDivisor = 1      ///< Damage divisor (for splash damage / glave wurm calculations)
                  );

  /// Deals damage directly to unit HP, killing it if possible.
  void damageHp(s32 damage, CUnit *attacker = NULL, s32 attackingPlayer = -1,
                bool notify = true);

  /// Reduces Defensive Matrix by @p amount, removing it if possible.
  void reduceDefensiveMatrixHp(s32 amount);

  /// Removes the unit from the game.
  void remove();

  /// Issue the @p order to the unit, using the given @p target unit.
  void orderTo(u8 orderId, const CUnit *target = NULL);

  /// Issue the @p order to the unit, using the given position as the target.
  void orderTo(u8 orderId, u16 x, u16 y);

  /// Causes the unit to become idle.
  void orderToIdle();

  /// Issues a new order to the unit.
  void order(u8 orderId, u16 x, u16 y, const CUnit *target, u16 targetUnitId, bool stopPreviousOrders);

  /// Used by several hooks.
  void setSecondaryOrder(u8 orderId);

  /// Makes the unit use the specified weapon to attack its current target unit
  /// stored in the CUnit::orderTarget.unit member. This does not affect the
  /// unit's weapon cooldown. The spawned weapon sprite obeys the weapon
  /// behavior properties in weapons.dat.
  /// Note: This function is affected by the Fire Weapon hook module.
  void fireWeapon(u8 weaponId) const;

  /// Checks whether this unit can reach the @p target unit. This checks only
  /// for terrain, and does not consider obstacles (units and buildings).
  bool hasPathToUnit(const CUnit *target) const;

  /// Checks whether this unit can reach the target position. This checks only
  /// for terrain, and does not consider obstacles (units and buildings).
  bool hasPathToPos(u32 x, u32 y) const;

  /// Checks the following:
  ///  * If the unit is actually owned by the commanding @p playerId
  ///  * If the unit's owning player has the @p techId researched
  ///  * If the unit has enough energy (or energy cheat is enabled)
  ///  * If the unit is not stunned / is a hallucination / is being built
  ///  * If the unit meets the Use Tech Requirements (editable in FireGraft)
  ///
  /// If the unit can use the tech, returns 1. If the tech needs to be
  /// researched, returns -1. If the unit cannot use the tech at all, returns 0.
  int canUseTech(u8 techId, s8 playerId) const;

  /// Checks if the @p unit can build / train / morph into @p unitId. This checks:
  /// * If the unit is actually owned by the commanding @p playerId
  /// * Whether the unit is stunned / disabled.
  ///
  /// If the unit can build / train / morph, returns 1. If the tech needs to be
  /// researched, returns -1. If the unit cannot use the tech at all, returns 0.
  int canBuild(u16 unitId, s8 playerId) const;

  /// Checks if the unit is a clean detector (no Lockdown, Optical Flare, etc.)
  bool canDetect() const;

  /// Checks if the unit is unpowered / lockdowned / stasised / maelstromed.
  bool isFrozen() const;

  /// Checks if the unit is a spellcaster (has energy) and not a hallucination.
  bool isValidCaster() const;

  /// Check if the unit is a remorphing building (i.e. is a Lair, Hive,
  /// Greater Spire, Sunken or Spore Colony under construction).
  bool isRemorphingBuilding() const;

  /// Returns the distance between this unit and the @p target, taking unit
  /// collision size in units.dat into account.
  /// Internally, this function uses scbw::getDistanceFast().
  u32 getDistanceToTarget(const CUnit *target) const;

  /// Returns the maximum range of a weapon in pixels. The weapon is assumed to
  /// be attached to this unit for calculating upgrade effects.
  /// This is affected by the Weapon Range hook module.
  u32 getMaxWeaponRange(u8 weaponId) const;

  /// Returns the unit's seek range (AKA target acquisition range) in matrices.
  /// This is affected by the Weapon Range hook module.
  u8 getSeekRange() const;

  /// Returns the sight range of this unit (with upgrades).
  /// If @p isForSpellCasting is true, also factors in status effects.
  u32 getSightRange(bool isForSpellCasting = false) const;

  /// Returns the maximum energy amount of this unit (with upgrades).
  u16 getMaxEnergy() const;

  /// Returns the armor count of this unit (with upgrades).
  u8 getArmor() const;

  /// Returns the bonus armor this unit has (from upgrades).
  u8 getArmorBonus() const;

  /// Returns the unit's ground weapon ID. If the unit is an unburrowed Lurker,
  /// returns WeaponId::None instead.
  u8 getActiveGroundWeapon() const;

  /// Updates the unit's actual speed. This function should be called after
  /// changing any properties and status effects that affect movement speed.
  void updateSpeed();

  /// Makes the unit's sprite play the specified Iscript animation entry.
  void playIscriptAnim(IscriptAnimation::Enum animation);

  /// Removes status effects from the unit (including the image overlays) and
  /// updates its button set.
  void removeLockdown();
  void removeMaelstrom();
  void removeStasisField();

  /// Removes the first image overlay with an ID value between @p imageIdStart
  /// and @p imageIdEnd.
  void removeOverlay(u32 imageIdStart, u32 imageIdEnd);

  /// Removes the first image overlay with the given image ID.
  void removeOverlay(u32 imageId);

  /// Returns the overlay image of this unit's sprite (or its subunit's sprite)
  /// that has the given @p imageId. If the image cannot be found, returns NULL.
  CImage* getOverlay(u16 imageId) const;

  u16 getX() const;
  u16 getY() const;
  u16 getTileX() const;
  u16 getTileY() const;

  /// Returns the absolute position of each of the unit's collision bounds.
  s16 getLeft() const;
  s16 getRight() const;
  s16 getTop() const;
  s16 getBottom() const;

  /// Retrieves the in-game name of this unit from stat_txt.tbl.
  const char* getName() const;

  /// Retrieves the unit pointer by @p index from the unit table (first unit is
  /// indexed at 1). If invalid, returns NULL instead.
  static CUnit* getFromIndex(u16 index);

  /// Returns the index of this unit in the unit table. First unit == index 1.
  u16 getIndex() const;

  /// Returns the ID of the last player to own this unit. This is usually the
  /// same as CUnit::playerId, but if the unit belongs to a defeated player,
  /// this returns the correct player ID (instead of 11).
  s8 getLastOwnerId() const;

  /// Checks whether this unit can be seen by @playerId (i.e. not covered by the
  /// fog-of-war and is detectable).
  bool isVisibleTo(s8 playerId) const;

  /// Returns the loaded unit at @p index (value between 0-7). If no unit is
  /// loaded at the slot, returns NULL instead.
  CUnit* getLoadedUnit(int index) const;

  /// Checks if this unit has other units loaded inside.
  bool hasLoadedUnit() const;
};

static_assert(sizeof(CUnit) == sizeof(CUnitLayout), "The size of the CUnit structure is invalid");
