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

    /// Searches the area given by (@p left, @p top, @p right, @p bottom),
    /// returning the nearest unit to @p sourceUnit for which match(unit)
    /// evaluates to true. If there are no matches, returns nullptr.
    /// This does not use unit collision boxes for calculating distances.
    template <class Callback>
    static CUnit* getNearestTarget(int left, int top, int right, int bottom,
                                   const CUnit* sourceUnit, Callback& match);
    
    /// Searches the entire map, returning the nearest unit to @p sourceUnit
    /// for which match(unit) evaluates to true. If there are no matches,
    /// returns nullptr.
    /// This does not use unit collision boxes for calculating distances.
    template <class Callback>
    static CUnit* getNearestTarget(const CUnit* sourceUnit, Callback& match);

  private:
    //This function is meant to be used by other getNearest() functions.
    //Do NOT use this function in the game code!
    template <class Callback>
    static CUnit* getNearest(int x, int y,
      int boundsLeft, int boundsTop, int boundsRight, int boundsBottom,
      UnitFinderData* left,  UnitFinderData* top,
      UnitFinderData* right, UnitFinderData* bottom,
      Callback &match, const CUnit *sourceUnit);
    
    static UnitFinderData* getStartX();
    static UnitFinderData* getStartY();
    static UnitFinderData* getEndX();
    static UnitFinderData* getEndY();

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

//-------- UnitFinder::getNearest() family --------//

//Based on function @ 0x004E8320
template <class Callback>
CUnit* UnitFinder::getNearest(int x, int y,
  int boundsLeft, int boundsTop, int boundsRight, int boundsBottom,
  UnitFinderData* left,  UnitFinderData* top,
  UnitFinderData* right, UnitFinderData* bottom,
  Callback &match, const CUnit *sourceUnit)
{
  using scbw::getDistanceFast;

  int bestDistance = getDistanceFast(0, 0,
    std::max(x - boundsLeft, boundsRight - x),
    std::max(y - boundsTop, boundsBottom - y));

  CUnit *bestUnit = nullptr;
  bool canContinueSearch, hasFoundBestUnit;

  do {
    canContinueSearch = false;
    hasFoundBestUnit = false;
    
    //Search to the left
    if (getStartX() <= left) {
      CUnit *unit = CUnit::getFromIndex(left->unitIndex);
      
      if (boundsLeft <= unit->getX()) {
        if (boundsTop <= unit->getY() && unit->getY() < boundsBottom) {
          if (unit != sourceUnit && match(unit)) {
            int distance = getDistanceFast(x, y, unit->getX(), unit->getY());
            if (distance < bestDistance) {
              bestDistance = distance;
              bestUnit = unit;
              hasFoundBestUnit = true;
            }
          }
        }
      }
      else
        left = getStartX() - 1;
      
      canContinueSearch = true;
      --left;
    }
    
    //Search to the right
    if (right < getEndX()) {
      CUnit *unit = CUnit::getFromIndex(right->unitIndex);
      
      if (unit->getX() < boundsRight) {
        if (boundsTop <= unit->getY() && unit->getY() < boundsBottom) {
          if (unit != sourceUnit && match(unit)) {
            int distance = getDistanceFast(x, y, unit->getX(), unit->getY());
            if (distance < bestDistance) {
              bestDistance = distance;
              bestUnit = unit;
              hasFoundBestUnit = true;
            }
          }
        }
      }
      else
        right = getEndX();
      
      canContinueSearch = true;
      ++right;
    }
    
    //Search upwards
    if (getStartY() <= top) {
      CUnit *unit = CUnit::getFromIndex(top->unitIndex);
      
      if (boundsTop <= unit->getY()) {
        if (boundsLeft <= unit->getX() && unit->getX() < boundsRight) {
          if (unit != sourceUnit && match(unit)) {
            int distance = getDistanceFast(x, y, unit->getX(), unit->getY());
            if (distance < bestDistance) {
              bestDistance = distance;
              bestUnit = unit;
              hasFoundBestUnit = true;
            }
          }
        }
      }
      else
        top = getStartY() - 1;
      
      canContinueSearch = true;
      --top;
    }
    
    //Search downwards
    if (bottom < getEndY()) {
      CUnit *unit = CUnit::getFromIndex(bottom->unitIndex);
      
      if (unit->getY() < boundsBottom) {
        if (boundsLeft <= unit->getX() && unit->getX() < boundsRight) {
          if (unit != sourceUnit && match(unit)) {
            int distance = getDistanceFast(x, y, unit->getX(), unit->getY());
            if (distance < bestDistance) {
              bestDistance = distance;
              bestUnit = unit;
              hasFoundBestUnit = true;
            }
          }
        }
      }
      else
        bottom = getEndY();
      
      canContinueSearch = true;
      ++bottom;
    }

    //Reduce the search bounds
    if (hasFoundBestUnit) {
      boundsLeft    = std::max(boundsLeft,    x - bestDistance);
      boundsRight   = std::min(boundsRight,   x + bestDistance);
      boundsTop     = std::max(boundsTop,     y - bestDistance);
      boundsBottom  = std::max(boundsBottom,  y + bestDistance);
    }
  } while (canContinueSearch);

  return bestUnit;
}

template <class Callback>
CUnit* UnitFinder::getNearestTarget(int left, int top, int right, int bottom,
  const CUnit* sourceUnit, Callback& match)
{
  UnitFinderData *searchLeft, *searchTop, *searchRight, *searchBottom;

  //If the unit sprite is hidden
  if (sourceUnit->sprite->flags & 0x20) {
    UnitFinderData temp;
    temp.position = sourceUnit->getX();
    searchRight   = std::lower_bound(getStartX(), getEndX(), temp);
    searchLeft    = searchRight - 1;
    temp.position = sourceUnit->getY();
    searchBottom  = std::lower_bound(getStartY(), getEndY(), temp);
    searchTop     = searchBottom - 1;
  }
  else {
    searchLeft    = getStartX() + sourceUnit->finderIndex.right - 1;
    searchRight   = getStartX() + sourceUnit->finderIndex.left + 1;
    searchTop     = getStartY() + sourceUnit->finderIndex.bottom - 1;
    searchBottom  = getStartY() + sourceUnit->finderIndex.top + 1;
  }

  return getNearest(sourceUnit->getX(), sourceUnit->getY(),
    left, top, right, bottom,
    searchLeft, searchTop, searchRight, searchBottom,
    match, sourceUnit);
}

template <class Callback>
CUnit* UnitFinder::getNearestTarget(const CUnit* sourceUnit, Callback& match) {
  return getNearestTarget(0, 0, mapTileSize->width * 32, mapTileSize->height * 32,
    sourceUnit, match);
}

} //scbw
