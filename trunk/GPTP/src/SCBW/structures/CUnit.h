//Based on BWAPI's BW/CUnit.h

#pragma once
#include "Target.h"
#pragma pack(1)

struct CSprite;
struct COrder;

//Comments marked with ++ added by RavenWolf
struct CUnit {
// Utility method definitions added by pastelmind

  /// Sets the unit's HP, and also updates burning / bleeding graphics and unit
  /// strength data (used by the AI). HP is guaranteed not to overflow.
  void setHp(s32 hitPoints);

  /// Deals damage directly to unit HP, killing it if possible.
  void damageHp(s32 damage, CUnit *attacker = NULL, s32 attackingPlayer = -1,
                bool notify = true);

  /// Reduces Defensive Matrix by @p amount, removing it if possible.
  void reduceDefensiveMatrixHp(s32 amount);

  /// Removes the unit from the game.
  void remove();

  /// Issue the @p order to the unit, using the given @p target unit.
  void orderTo(u32 orderId, const CUnit *target);

  /// Issue the @p order to the unit, using the given position as the target.
  void orderTo(u32 orderId, u16 x, u16 y);

  /// Checks whether this unit can reach the @p target unit. This checks only
  /// for terrain, and does not consider obstacles (units and buildings).
  bool hasPathToUnit(const CUnit *target);

  /// Checks whether this unit can reach the target position. This checks only
  /// for terrain, and does not consider obstacles (units and buildings).
  bool hasPathToPos(u32 x, u32 y);

  /// Returns the distance between this unit and the @p target, taking unit
  /// collision size in units.dat into account.
  /// Internally, this function uses scbw::getDistanceFast().
  u32 getDistanceToTarget(const CUnit *target) const;

  /// Updates the unit's actual speed. This function should be called after
  /// changing any properties and status effects that affect movement speed.
  void updateSpeed();

  /// Makes the unit's sprite play the specified Iscript animation entry.
  ///
  /// @param  animation   ID of the Iscript animation to play. See
  ///                     IscriptAnimation::Enum for a list of animations.
  void playIscriptAnim(u8 animation);

  /// Removes status effects from the unit (including the image overlays) and
  /// updates its button set.
  void removeLockdown();
  void removeMaelstrom();
  void removeStasisField();

  u16 getX() const;
  u16 getY() const;
  u16 getTileX() const;
  u16 getTileY() const;

  /// Returns the absolute position of each of the unit's collision bounds.
  s16 getLeft() const;
  s16 getRight() const;
  s16 getTop() const;
  s16 getBottom() const;


////////////////////////////////////////////////////////////////
// Actual data structure -- member variables and pointers
  CUnit     *previous;
  CUnit     *next;
  s32       hitPoints;
  CSprite   *sprite;
  Target    moveTarget;         ///< The position or unit to move to. It is NOT an order target.
  Position  nextMovementWaypoint; ///< The next way point in the path the unit is following to get to its destination.
                                  ///< Equal to moveToPos for air units since they don't need to navigate around buildings.
  Position  nextTargetWaypoint; ///< The desired position
  u8        movementFlags;
  u8        currentDirection1;
  u8        flingyTurnSpeed;    //AKA flingy turn radius
  u8        velocityDirection1;
  u16       flingyId;
  UNK unknown2;
  u8        flingyMovementType;
  Position  position;
  u32       xHalt;
  u32       yHalt;
  u32       flingyTopSpeed;     //top speed
  u32       current_speed1;     //++current speed
  u32       current_speed2;     //++not sure if there are any diference with this 2 variables, they always have the same value
  UNK unknown3[8];
  u16       flingyAcceleration;
  u8        currentDirection2;  //?
  u8        velocityDirection2; //?
  u8        playerId;
  u8        mainOrderId;
  u8        mainOrderState;
  u8        orderSignal;        /*  0x01  Update building graphic/state
                                    0x02  Casting spell
                                    0x04  Reset collision? Always enabled for hallucination...
                                    0x10  Lift/Land state
                                */
  u16       orderUnitType;
  UNK unknown5[2];
  u8        mainOrderTimer;
  u8        groundWeaponCooldown;
  u8        airWeaponCooldown;
  u8        spellCooldown;
  Target    orderTarget;
  s32       shields;            // Although BWAPI says this is unsigned, StarCraft.exe uses signed comparisons for this value
  u16       id;                 // Unit ID in units.dat
  UNK unknown6[2];
  CUnit*    playerPrevious;
  CUnit*    playerNext;
  CUnit*    subunit;
  COrder*   orderQueueHead;
  COrder*   orderQueueTail;
  CUnit*    autoTargetUnit;
  CUnit*    connectedUnit;      // ++Larva store a reference to the hatchery/Lair/Hive who created it
                                // ++units inside transports store a reference to the transport
                                // addons store a reference to its parent building (but not the other way around)
  u8        orderQueueCount;
  u8        orderQueueTimer;    //?
  UNK unknown8;
  u8        attackNotifyTimer;  // Prevent "Your forces are under attack." on every attack
  u16       displayedUnitId;    //++used by zerg buildings while morphing
  u8        lastEventTimer;     // countdown that stops being recent when it hits 0
  u8        lastEventColor;     // 17 = was completed (train, morph), 174 = was attacked

  UNK unknown9[2];
  u8        rankIncrease;
  u8        killCount;
  u8        lastAttackingPlayer;// the player that last attacked this unit
  u8        secondaryOrderTimer;
  u8        aiActionFlag;       // Used internally by the AI
  u8        userActionFlags;    // some flags that change when the user interacts with the unit
                                // 2 = issued an order, 3 = interrupted an order, 4 = hide self before death
  u16       currentButtonSet;   // The u16 is a guess, used to be u8
  u8        isCloaked;          // 1 for cloaked
  u8        movementState;      // A value based on conditions related to pathing, see Path.h for info

  u16       buildQueue[5];    //++train queue for buildings and carrier/reaver (stores the unitid) / zerg eggs stores the id of the morphing unit in queue[0]
  u16       energy;
  u8        buildQueueSlot;     //< Index of active unit in #buildQueue.
  u8        targetOrderSpecial; //< A byte used to determine the target ID for the unit
  u8        secondaryOrderId;
  u8        buildingOverlayState; // 0 means the building has the largest amount of fire/blood
  u16       buildRepairHPGain;  //++hp ammount increased per frame when a unit/building its constructed/repaired
  UNK unknown14[2];
  u16       remainingBuildTime;
  u16       previousHp;         // The HP of the unit before it changed (example Drone->Hatchery, the Drone's HP will be stored here)
  u16       loadedUnitIndex[8];

  union {   //0x0C0
    struct {
      u8 spiderMineCount; // 0
    } vulture;

    struct {
      CUnit*  inHangerChild;    // 0   first child inside the hanger
      CUnit*  outHangerChild;   // 4   first child outside the hanger
      u8      inHangerCount;    // 8   number inside the hanger
      u8      outHangerCount;   // 9   number outside the hanger
    } carrier; // also applies to reaver

    struct {
      CUnit*  parent;   // 0
      CUnit*  prev;     // 4
      CUnit*  next;     // 8
      u8      inHanger;  // C
    } interceptor;  // also applies to scarab

    struct {
      u32 _unknown_00;
      u32 _unknown_04;
      u32 flagSpawnFrame; // flag beacons, the frame that the flag will spawn
    } beacon;

    struct {
      CUnit*  addon;                // 0
      u16     addonBuildType;       // 4
      u16     upgradeResearchTime;  // 6
      u8      techType;             // 8
      u8      upgradeType;          // 9
      u8      larvaTimer;           // A
      u8      landingTimer;         // B
      u8      creepTimer;           // C
      u8      upgradeLevel;         // D
      u16     _padding_0E;          // E    // possible alignment
      union {
        struct {
          u16       resourceCount;  // amount of resources
          u8        resourceIscript;
          u8        gatherQueueCount;
          CUnit*    nextGatherer;   // 14  // pointer to the next workerunit waiting in line to gather
          u8        resourceGroup;  // 18
          u8        resourceBelongsToAI;
        } resource;  /** When the unit is resource container */
        struct { CUnit* exit; } nydus; /** connected nydius canal */
        struct { CSprite* nukeDot; } ghost;
        struct { CSprite* pylonAura; } pylon;
        struct{
          CUnit* nuke;  // attached nuke
          u32 hasNuke;     // 14
        } silo;
        struct{
          u16 harvestValue1;
          u16 harvestValue2;
          u16 _unknown_14;    // 14
          u16 harvestValue3;
        } hatchery; // wtf???
        struct {
          u16  powerupX;
          u16  powerupY;
        } powerup;
      };
    } building;

    struct {
      CUnit*  powerup;                // 0
      u16     targetResourceX;        // 4
      u16     targetResourceY;
      CUnit*  targetResourceUnit;     // 8
      u16     repairResourceLossTimer;// C
      u8      isCarryingSomething;    // E
      u8      resourceCarryCount;     // F
      CUnit*  harvestTarget;          // 10
      CUnit*  prevHarvestUnit;        // 14   // When there is a gather conflict
      CUnit*  nextHarvestUnit;        // 18
    } worker;
  };

  u32       status;             // See UnitStatus::Enum for more info
  u8        resourceType;
  u8        wireframeRandomizer;
  u8        secondaryOrderState;
  u8        recentOrderTimer;   // Counts down from 15 to 0 when most orders are given, or when the unit moves after reaching a patrol location
  s32       visibilityStatus;   // Flags specifying which players can detect this unit (cloaked/burrowed)
  u16       _unknown_0x0E8;     // Secondary order related (x?)
  u16       _unknown_0x0EA;     // Secondary order related (y?)
  CUnit*    currentBuildUnit;
  CUnit*    previousBurrowedUnit;
  CUnit*    nextBurrowedUnit;
  union {
    Target rally;               // This is not present in BWAPI's CUnit.h, but it seems pretty obvious that it is a Target struct.
    struct {
      CUnit* previousPsiProvider;
      CUnit* nextPsiProvider;
    } pylon;
  };
  u32       path;               //?
  u8        pathingCollisionInterval;  // unknown
  u8        pathingFlags;       // 0x01 = uses pathing; 0x02 = ?; 0x04 = ?
  u8        unused_0x106;
  u8        isBeingHealed;
  u16       contours1XUnknown;  //?
  u16       contours1YUnknown;  //?
  u16       contours2XUnknown;  //?
  u16       contours2YUnknown;  //?
  u16       removeTimer;        // Hallucination, Dark Swarm, Disruption Web, Broodling
  u16       defensiveMatrixHp;
  u8        defensiveMatrixTimer;
  u8        stimTimer;
  u8        ensnareTimer;
  u8        lockdownTimer;
  u8        irradiateTimer;
  u8        stasisTimer;
  u8        plagueTimer;
  u8        isUnderStorm;       // Used to tell if a unit is under psi storm
  CUnit*    irradiatedBy;
  u8        irradiatePlayerId;
  u8        parasiteFlags;      // Each bit corrisponds to the player who has parasited this unit*/
  u8        cycleCounter;       // counts/cycles up from 0 to 7 (inclusive).
  u8        isBlind;
  u8        maelstromTimer;
  u8        unusedTimer;        // ?? Might be afterburner timer or ultralisk roar timer
  u8        acidSporeCount;
  u8        acidSporeTime[9];
  u16       offsetIndex3by3;
  u16       _padding_0x132;
  void*     pAI;
  u16       airStrength;
  u16       groundStrength;
  u32       tableIdUnknown[4];        //?
  u8        repulse1Unknown;        //?
  u8        repulseAngle;          // updated only when air unit is being pushed
  u8        driftPosX;
  u8        driftPosY;
};

C_ASSERT(sizeof(CUnit) == 336);


/*
struct WEAPON {
  WEAPON*      previousWeapon;
  WEAPON*      nextWeapon;
  UNK unknown1[4];
  SPRITE*      sprite;
  u16        targetGroundX;
  u16        targetGroundY;
  CUnit*      targetUnit;
  u8        weaponId;
  UNK unknown2[2];
  u8        bounceCount;
  CUnit*      attackingUnit;
};*/


#pragma pack()
