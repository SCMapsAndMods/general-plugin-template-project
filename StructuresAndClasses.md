GPTP provides many structures and classes that mirror the layout of the data structures used by StarCraft. To use the capabilities of GPTP to the fullest, one must understand how these work.

# Primary Structures #

There are several primary structures that contain and control how the game works:

  * <tt><a href='CUnit.md'>CUnit</a></tt>: This structure represents an in-game unit, such as a Marine, Zergling, or Zealot. Each `CUnit` object, if used, has one associated `CSprite` object, pointed by `CUnit::sprite`.
  * `CBullet`: Represents a bullet object fired by a unit, used for normal attacks and spell projectiles. Each `CBullet` object, if used, has one associated `CSprite` object, pointed by `CBullet::sprite`.
  * `CSprite`: Represents a sprite, which is a collection of images (`CImage` objects) that work together. Each `CSprite` object, if used, has one or more associated `CImage` objects.
  * `CImage`: Represents an image, which consists of various information on how a GRP file should be displayed and animated in-game.

(To be expanded)



# Supplementary Structures #

Along with primary structures, StarCraft uses data structures to organize all sorts of information. The following are some of the more useful structures that a modder should know about.

(To be expanded)

## `UnitGroup` ##

This structure is actually a bitfield that determines whether a type of unit (e.g. Marine, Vulture, Mutalisk) belongs to a specific unit group defined in `units.dat`. This bitfield is used when retrieving data from the `units_dat::UnitGroup[]`.

```
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
```

## `ActiveTile` ##

This structure is a collection of data flags and bitfields containing information about a map tile (32px × 32px map section). This is used when accessing map tile data with <tt><a href='ApiFunctions#getActiveTileAt.md'>scbw::getActiveTileAt()</a></tt>.

```
struct ActiveTile {
  u8 visibilityFlags;        // Each bit is set if the player does NOT have vision of the current tile.
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
```