#pragma once
#include "scbwdata.h"
#include <algorithm>

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
    
    /// Iterates through all units found, calling func() once for each unit.
    template <class Callback>
    void forEach(Callback &func) const;
    
    /// Returns the first unit for which match() returns true.
    /// If there are no matches, returns nullptr.
    template <class Callback>
    CUnit* getFirst(Callback &match) const;
    
    /// Returns the unit for which score() returns the highest nonnegative
    /// integer. If there are no units, returns nullptr.
    /// Note: If score() returns a negative value, the unit is ignored.
    template <class Callback>
    CUnit* getBest(Callback &score) const;

    /// Returns the unit nearest to (x, y) for which match() returns true, using
    /// the given map bounds. If there are no matches, returns nullptr.
    /// This does NOT take unit size into account when calculating distance.
    template <class Callback>
    static CUnit* getNearest(int x, int y, int left, int top, int right, int bottom,
                             Callback &match);
    
    /// Returns the unit nearest to (x, y) for which match() returns true,
    /// searching the entire map. If there are no matches, returns nullptr.
    /// This does NOT take unit size into account when calculating distance.
    template <class Callback>
    static CUnit* getNearest(int x, int y, Callback &match);

  private:
    int unitCount;
    CUnit* units[UNIT_ARRAY_LENGTH];
};


//-------- Template member function definitions --------//

template <class Callback>
void UnitFinder::forEach(Callback &func) const {
  for (int i = 0; i < this->getUnitCount(); ++i)
    func(this->getUnit(i));
}

template <class Callback>
CUnit* UnitFinder::getFirst(Callback &match) const {
  for (int i = 0; i < this->getUnitCount(); ++i)
    if (match(this->getUnit(i)))
      return this->getUnit(i);
  return nullptr;
}

template <class Callback>
CUnit* UnitFinder::getBest(Callback &score) const {
  int bestScore = -1;
  CUnit *bestUnit = nullptr;

  for (int i = 0; i < this->getUnitCount(); ++i) {
    const int score = score(this->getUnit(i));
    if (score > bestScore) {
      bestUnit = this->getUnit(i);
      bestScore = score;
    }
  }

  return bestUnit;
}

template <class Callback>
CUnit* UnitFinder::getNearest(int x, int y, int left, int top, int right, int bottom,
                              Callback &match) {
  // Obtain finder indexes for all bounds
  UnitFinderData* const p_xbegin = unitOrderingX;
  UnitFinderData* const p_ybegin = unitOrderingY;
  UnitFinderData* const p_xend = unitOrderingX + *unitOrderingCount;
  UnitFinderData* const p_yend = unitOrderingY + *unitOrderingCount;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.position = x;
  UnitFinderData *pLeft = std::lower_bound(p_xbegin, p_xend, finderVal);
  UnitFinderData *pRight = pLeft + 1;

  finderVal.position = y;
  UnitFinderData *pTop = std::lower_bound(p_ybegin, p_yend, finderVal);
  UnitFinderData *pBottom = pTop + 1;

  CUnit *bestUnit = nullptr;
  int bestDistance = 999999;
  bool canContinue, canNarrowSearchBounds;
  bool isUnitVisited[UNIT_ARRAY_LENGTH + 1] = {false};

  do {
    canContinue = false;
    canNarrowSearchBounds = false;

    if (pLeft >= p_xbegin && pLeft->position >= left) {
      if (!isUnitVisited[pLeft->unitIndex]) {
        isUnitVisited[pLeft->unitIndex] = true;
        CUnit *unit = CUnit::getFromIndex(pLeft->unitIndex);
        if (left <= unit->getX() && unit->getX() < right
            && top <= unit->getY() && unit->getY() < bottom
            && match(unit))
        {
          int distance = scbw::getDistanceFast(x, y, unit->getX(), unit->getY());
          if (distance < bestDistance) {
            bestUnit = unit;
            bestDistance = distance;
            canNarrowSearchBounds = true;
          }
        }
      }
      --pLeft;
      canContinue = true;
    }

    if (pRight < p_xend && pRight->position <= right) {
      if (!isUnitVisited[pRight->unitIndex]) {
        isUnitVisited[pRight->unitIndex] = true;
        CUnit *unit = CUnit::getFromIndex(pRight->unitIndex);
        if (left <= unit->getX() && unit->getX() < right
            && top <= unit->getY() && unit->getY() < bottom
            && match(unit))
        {
          int distance = scbw::getDistanceFast(x, y, unit->getX(), unit->getY());
          if (distance < bestDistance) {
            bestUnit = unit;
            bestDistance = distance;
            canNarrowSearchBounds = true;
          }
        }
      }
      ++pRight;
      canContinue = true;
    }
    
    if (pTop >= p_ybegin && pTop->position >= top) {
      if (!isUnitVisited[pTop->unitIndex]) {
        isUnitVisited[pTop->unitIndex] = true;
        CUnit *unit = CUnit::getFromIndex(pTop->unitIndex);
        if (left <= unit->getX() && unit->getX() < right
            && top <= unit->getY() && unit->getY() < bottom
            && match(unit))
        {
          int distance = scbw::getDistanceFast(x, y, unit->getX(), unit->getY());
          if (distance < bestDistance) {
            bestUnit = unit;
            bestDistance = distance;
            canNarrowSearchBounds = true;
          }
        }
      }
      --pTop;
      canContinue = true;
    }
    
    if (pBottom < p_yend && pBottom->position < bottom) {
      if (!isUnitVisited[pBottom->unitIndex]) {
        isUnitVisited[pBottom->unitIndex] = true;
        CUnit *unit = CUnit::getFromIndex(pBottom->unitIndex);
        if (left <= unit->getX() && unit->getX() < right
            && top <= unit->getY() && unit->getY() < bottom
            && match(unit))
        {
          int distance = scbw::getDistanceFast(x, y, unit->getX(), unit->getY());
          if (distance < bestDistance) {
            bestUnit = unit;
            bestDistance = distance;
            canNarrowSearchBounds = true;
          }
        }
      }
      ++pBottom;
      canContinue = true;
    }

    //Narrow down search boundaries
    if (canNarrowSearchBounds) {
      left   = std::max(left,   x - bestDistance);
      top    = std::max(top,    y - bestDistance);
      right  = std::min(right,  x + bestDistance);
      bottom = std::min(bottom, y + bestDistance);
    }
  } while (canContinue);

  return bestUnit;
}

template <class Callback>
CUnit* UnitFinder::getNearest(int x, int y, Callback &match) {
  return getNearest(x, y, 0, 0, mapTileSize->width * 32, mapTileSize->height * 32, match);
}

} //scbw
