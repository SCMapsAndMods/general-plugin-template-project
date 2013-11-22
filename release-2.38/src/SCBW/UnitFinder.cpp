#include "UnitFinder.h"
#include "api.h"
#include <algorithm>
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

void UnitFinder::forEach(scbw::UnitFinderCallbackProcInterface &callback) const {
  for (int i = 0; i < this->unitCount; ++i)
    callback.proc(this->units[i]);
}

CUnit* UnitFinder::getFirst(UnitFinderCallbackMatchInterface &callback) const {
  for (int i = 0; i < this->unitCount; ++i)
    if (callback.match(this->units[i]))
      return this->units[i];

  return NULL;
}

CUnit* UnitFinder::getBest(scbw::UnitFinderCallbackScoreInterface &callback) const {
  int bestScore = -1;
  CUnit *bestUnit = NULL;

  for (int i = 0; i < this->unitCount; ++i) {
    const int currentScore = callback.score(this->units[i]);
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestUnit = this->units[i];
    }
  }

  return bestUnit;
}

//Based on function @ 0x004E8320
CUnit* UnitFinder::getNearest(int x, int y, int left, int top, int right, int bottom,
                              UnitFinderCallbackMatchInterface &callback) {
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

  CUnit *bestUnit = NULL;
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
            && callback.match(unit)) {
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
            && callback.match(unit)) {
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
            && callback.match(unit)) {
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
            && callback.match(unit)) {
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

CUnit* UnitFinder::getNearest(int x, int y, UnitFinderCallbackMatchInterface &callback) {
  return getNearest(x, y, 0, 0, mapTileSize->width * 32, mapTileSize->height * 32, callback);
}

} //scbw
