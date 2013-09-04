#include "unit_finder.h"
#include <algorithm>

namespace scbw {

//Based on BWAPI's Shared/Templates.h
void findUnitsInBounds(int left, int top, int right, int bottom,
                       UnitFinderResult &unitsFound) {
  u32 finderFlags[UNIT_ARRAY_LENGTH + 1] = {0}; //1701 elements

  int r = right, b = bottom;
  bool isWidthExtended  = right - left - 1 < *maxUnitWidth;
  bool isHeightExtended = top - bottom - 1 < *maxUnitHeight;

  // Check if the location is smaller than the largest unit
  if (isWidthExtended)
    r += *maxUnitWidth;
  if (isHeightExtended)
    b += *maxUnitHeight;

  // Obtain finder indexes for all bounds
  UnitFinderData *p_xend = unitOrderingX + *unitOrderingCount;
  UnitFinderData *p_yend = unitOrderingY + *unitOrderingCount;

  // Create UnitFinderData elements for compatibility with stl functions
  UnitFinderData finderVal;

  // Search for the values using built-in binary search algorithm and comparator
  finderVal.searchValue = left;
  UnitFinderData *pLeft   = std::lower_bound<UnitFinderData*>(unitOrderingX, p_xend, finderVal);

  finderVal.searchValue = top;
  UnitFinderData *pTop    = std::lower_bound<UnitFinderData*>(unitOrderingY, p_yend, finderVal);

  finderVal.searchValue = r - 1;
  UnitFinderData *pRight  = std::upper_bound<UnitFinderData*>(pLeft, p_xend, finderVal);

  finderVal.searchValue = b - 1;
  UnitFinderData *pBottom = std::upper_bound<UnitFinderData*>(pTop, p_yend, finderVal);

  // Iterate the X entries of the finder
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    const int actualUnitIndex = px->unitIndex - 1;
    if (finderFlags[actualUnitIndex] == 0) {
      if (isWidthExtended) {  // If width is small, check unit bounds
        CUnit *unit = &unitTable[actualUnitIndex];
        if (unit->getLeft() <= right)
          finderFlags[actualUnitIndex] = 1;
      }
      else
        finderFlags[actualUnitIndex] = 1;
    }
  }

  // Iterate the Y entries of the finder
  for (UnitFinderData *py = pTop; py < pBottom; ++py) {
    const int actualUnitIndex = py->unitIndex - 1;
    if (finderFlags[actualUnitIndex] == 1) {
      if (isHeightExtended) { // If height is small, check unit bounds
        CUnit *unit = &unitTable[actualUnitIndex];
        if (unit->getTop() <= bottom)
          finderFlags[actualUnitIndex] = 2;
      }
      else
        finderFlags[actualUnitIndex] = 2;
    }
  }

  // Final iteration
  int unitsFoundCount = 0;
  for (UnitFinderData *px = pLeft; px < pRight; ++px) {
    const int actualUnitIndex = px->unitIndex - 1;
    if (finderFlags[actualUnitIndex] == 2) {
      CUnit *unit = &unitTable[actualUnitIndex];
      if (unit && unit->mainOrderId)
        unitsFound.units[unitsFoundCount++] = unit;
    }
  }

  unitsFound.count = unitsFoundCount;
}

} //scbw
