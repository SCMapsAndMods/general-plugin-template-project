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

UnitFinderData* UnitFinder::getStartX() {
  return unitOrderingX;
}

UnitFinderData* UnitFinder::getStartY() {
  return unitOrderingY;
}

UnitFinderData* UnitFinder::getEndX() {
  return unitOrderingX + *unitOrderingCount;
}

UnitFinderData* UnitFinder::getEndY() {
  return unitOrderingY + *unitOrderingCount;
}

// The heart and core of StarCraft's unit search engine.
// Based on BWAPI's Shared/Templates.h
void UnitFinder::search(int left, int top, int right, int bottom) {
  bool tempUnitStored[UNIT_ARRAY_LENGTH + 1] = {false};

  int r = right, b = bottom;
  const bool isWidthExtended  = right - left - 1 < *MAX_UNIT_WIDTH;
  const bool isHeightExtended = top - bottom - 1 < *MAX_UNIT_HEIGHT;

  // Check if the location is smaller than the largest unit
  if (isWidthExtended)
    r += *MAX_UNIT_WIDTH;
  if (isHeightExtended)
    b += *MAX_UNIT_HEIGHT;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.position = left;
  UnitFinderData *pLeft   = std::lower_bound(getStartX(), getEndX(), finderVal);

  finderVal.position = top;
  UnitFinderData *pTop    = std::lower_bound(getStartY(), getEndY(), finderVal);

  finderVal.position = r;
  UnitFinderData *pRight  = std::lower_bound(pLeft, getEndX(), finderVal);

  finderVal.position = b;
  UnitFinderData *pBottom = std::lower_bound(pTop, getEndY(), finderVal);

  // Iterate the X entries of the finder
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    if (!tempUnitStored[px->unitIndex]) {
      // If width is small, check unit bounds
      if (!isWidthExtended
          || CUnit::getFromIndex(px->unitIndex)->getLeft() < right)
        tempUnitStored[px->unitIndex] = true;
    }
  }

  // Iterate the Y entries of the finder
  this->unitCount = 0;
  for (UnitFinderData *py = pTop; py < pBottom; ++py) {
    if (tempUnitStored[py->unitIndex]) {
      tempUnitStored[py->unitIndex] = false;  //Prevent duplicates

      // If height is small, check unit bounds
      if (!isHeightExtended
          || CUnit::getFromIndex(py->unitIndex)->getTop() < bottom)
      {
        CUnit *unit = CUnit::getFromIndex(py->unitIndex);
        if (unit && unit->mainOrderId)
          this->units[this->unitCount++] = unit;
      }
    }
  }
}

} //scbw
