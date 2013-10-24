#pragma once
#include "scbwdata.h"

namespace scbw {

/// The UnitFinder class is used to efficiently search for units in a certain
/// area using StarCraft's internal data structures.

class UnitFinder {
  public:
    /// Default constructor.
    UnitFinder();

    /// Constructs and searches for all units within the given bounds.
    UnitFinder(int left, int top, int right, int bottom);

    /// Searches for all units within the given bounds, using StarCraft's
    /// internal data structures. This is much more efficient than manually
    /// looping through the entire unit table or linked list.
    void search(int left, int top, int right, int bottom);

    /// Returns the number of units found by UnitFinder::search() call.
    /// If no searches have been conducted, returns 0.
    int getUnitCount() const;

    /// Returns the unit at the given index. Invalid index returns NULL instead.
    CUnit* getUnit(int index) const;

    /// Iterates through all units found, calling @p proc() once for each unit.
    ///
    /// @param  proc    A callback function that accepts a CUnit pointer.
    template <typename callback>
    void forEach(callback &proc) const;

    /// Returns the first unit for which @p match() returns true.
    /// If there are no matches, returns NULL.
    ///
    /// @param  match   A callback function that accepts a CUnit pointer and
    ///                 returns true/false.
    template <typename callback>
    CUnit* getFirst(callback &match) const;

    /// Returns the unit for which @p score() returns the highest nonnegative
    /// score. If there are no units, returns NULL.
    ///
    /// @param  score   A callback function that accepts a CUnit pointer and
    ///                 returns an int value as the unit's "score". If the score
    ///                 is less than zero, the unit is ignored.
    template <typename callback>
    CUnit* getBest(callback &score) const;

    /// Returns the nearest unit to (x, y) for which @p match() returns true,
    /// using the given map bounds. If there are no matches, returns NULL.
    /// This does NOT take unit size into account when calculating distance.
    template <typename callback>
    static CUnit* getNearest(int x, int y, int left, int top, int right, int bottom, callback &match);

  private:
    int unitCount;
    CUnit* units[UNIT_ARRAY_LENGTH];
};


//-------- Template member function definitions --------//

template <typename callback>
void UnitFinder::forEach(callback &proc) const {
  for (int i = 0; i < this->unitCount; ++i)
    proc(this->units[i]);
}

template <typename callback>
CUnit* UnitFinder::getFirst(callback &match) const {
  for (int i = 0; i < this->unitCount; ++i)
    if (match(this->units[i]))
      return this->units[i];
  
  return NULL;
}

template <typename callback>
CUnit* UnitFinder::getBest(callback &score) const {
  int bestScore = -1;
  CUnit *bestUnit = NULL;

  for (int i = 0; i < this->unitCount; ++i) {
    const int currentScore = score(this->units[i]);
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestUnit = this->units[i];
    }
  }

  return bestUnit;    
}

template <typename callback>
static CUnit* UnitFinder::getNearest(int x, int y, int left, int top, int right, int bottom, callback &match) {
  // Obtain finder indexes for all bounds
  UnitFinderData *p_xend = unitOrderingX + *unitOrderingCount;
  UnitFinderData *p_yend = unitOrderingY + *unitOrderingCount;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.position = left;
  UnitFinderData *pLeft   = std::lower_bound(unitOrderingX, p_xend, finderVal);

  finderVal.position = top;
  UnitFinderData *pTop    = std::lower_bound(unitOrderingY, p_yend, finderVal);

  finderVal.position = right - 1;
  UnitFinderData *pRight  = std::upper_bound(pLeft, p_xend, finderVal);

  finderVal.position = bottom - 1;
  UnitFinderData *pBottom = std::upper_bound(pTop, p_yend, finderVal);

  return NULL;
}

} //scbw
