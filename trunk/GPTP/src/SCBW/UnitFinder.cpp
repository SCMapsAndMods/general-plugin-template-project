#include "UnitFinder.h"
#include "api.h"
#include <cassert>

namespace scbw {

UnitFinder::UnitFinder() : unitCount(0) {}

UnitFinder::UnitFinder(int left, int top, int right, int bottom)
: unitCount(0) {
  this->search(left, top, right, bottom);
}

int UnitFinder::getUnitCount() const {
  return this->unitCount;
}

CUnit* UnitFinder::getUnit(int index) const {
  if (0 <= index && index <= this->unitCount)
    return this->units[index];
  return NULL;
}

// The heart and core of StarCraft's unit search engine.
// Based on BWAPI's Shared/Templates.h
void UnitFinder::search(int left, int top, int right, int bottom) {
  u8 finderFlags[UNIT_ARRAY_LENGTH + 1] = {0};

  int r = right, b = bottom;
  const bool isWidthExtended  = right - left - 1 < *MAX_UNIT_WIDTH;
  const bool isHeightExtended = top - bottom - 1 < *MAX_UNIT_HEIGHT;

  // Check if the location is smaller than the largest unit
  if (isWidthExtended)
    r += *MAX_UNIT_WIDTH;
  if (isHeightExtended)
    b += *MAX_UNIT_HEIGHT;

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

  finderVal.position = r - 1;
  UnitFinderData *pRight  = std::upper_bound(pLeft, p_xend, finderVal);

  finderVal.position = b - 1;
  UnitFinderData *pBottom = std::upper_bound(pTop, p_yend, finderVal);

  // Iterate the X entries of the finder
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    if (finderFlags[px->unitIndex] == 0) {
      // If width is small, check unit bounds
      if (!isWidthExtended
          || CUnit::getFromIndex(px->unitIndex)->getLeft() <= right)
        finderFlags[px->unitIndex] = 1;
    }
  }

  // Iterate the Y entries of the finder
  this->unitCount = 0;
  for (UnitFinderData *py = pTop; py < pBottom; ++py) {
    if (finderFlags[py->unitIndex] == 1) {
      // If height is small, check unit bounds
      if (!isHeightExtended
          || CUnit::getFromIndex(py->unitIndex)->getTop() <= bottom)
        finderFlags[py->unitIndex] = 2;
    }
  }

  // Final iteration
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    if (finderFlags[px->unitIndex] == 2) {
      CUnit *unit = CUnit::getFromIndex(px->unitIndex);
      if (unit && unit->mainOrderId)
        this->units[this->unitCount++] = unit;
    }
    finderFlags[px->unitIndex] = 0; //Prevent duplicates
  }
}

} //scbw
