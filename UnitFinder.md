Class header file: [SCBW/UnitFinder.h](http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/SCBW/UnitFinder.h)

The `UnitFinder` class utilizies the internal data structures that StarCraft uses to efficiently search for units within a portion of the map. Several of its methods support callback functions and functors that can process these units.

Note: This class internally handles an array of 1700 <tt><a href='CUnit.md'>CUnit</a></tt> pointers, so instantiating it every time may become expensive. Consider declaring the `UnitFinder` variable as static.



# Methods #

## `UnitFinder` ##

```
UnitFinder();
UnitFinder(int left, int top, int right, int bottom);
```

Constructors for the `UnitFinder` class. The four-parameter version immediately calls [UnitFinder::search()](#search.md).

## `search` ##

```
void search(int left, int top, int right, int bottom);
```

Searches for all units within the given bounds, using StarCraft's internal data structures. This is much more efficient than manually looping through the entire unit table or linked list. The search result is stored in the current instance of the class, and can be used with other member functions.

## `getUnitCount` ##

```
int getUnitCount() const;
```

Returns the number of units found by [UnitFinder::search()](#search.md). If no searches have been conducted, returns 0.

## `getUnit` ##

```
CUnit* getUnit(int index) const;
```

Returns the unit at the given index. Invalid index returns `nullptr` instead.


## `forEach` ##

```
template <class Callback>
void forEach(Callback &func) const;
```

Iterates through all units found, calling `func()` once for each unit.

This example increases the HP of all units found by 10:
```
finder.forEach([] (CUnit *unit) {
  unit->setHp(unit->hitPoints + 2560);
});
```

## `getFirst` ##

```
template <class Callback>
CUnit* getFirst(Callback &match) const;
```

Returns the first unit for which `match()` returns true. If there are no matches, returns `nullptr`.

This example searches for a Zerg Defiler in an area:
```
CUnit *defiler = finder.getFirst([] (CUnit *unit) {
  if (unit->id == UnitId::defiler)
    return true;
  else
    return false;
});
```

## `getBest` ##

```
template <class Callback>
CUnit* getBest(Callback &score) const;
```

Returns the unit for which `score()` returns the highest nonnegative score. If a unit receives negative score, it is excluded from the search. If there are no units, returns `nullptr`.


This example searches for an Ultralisk with the most HP in an area:
```
CUnit *bestUltralisk = finder.getBest([] (CUnit *unit) -> int {
  if (unit->id == UnitId::ultralisk)
    return unit->hitPoints; //The unit's HP serves as the "score"
  else
    return -1; //Don't bother if the unit is not an Ultralisk
});
```

## `getNearestTarget` ##

```
template <class Callback>
static CUnit* getNearestTarget(int left, int top, int right, int bottom, const CUnit* sourceUnit, Callback& match);
template <class Callback>
static CUnit* getNearestTarget(const CUnit* sourceUnit, Callback& match);
```

Searches the area given by (`left`, `top`, `right`, `bottom`), returning the nearest unit to `sourceUnit` for which `match(unit)` evaluates to true. If there are no matches, returns `nullptr`.

The 3-parameter version searches the entire map. Depending on the number of units in the game, it may be significantly slower than the version that uses custom search boundaries.

This does not use unit collision boxes for calculating distances.

_Note: This is a static member function, and does NOT require an instance of the class._