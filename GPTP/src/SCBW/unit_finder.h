//Unit finder functions and containers.
//Algorithm based on BWAPI's Shared/Templates.h

#pragma once
#include "scbwdata.h"

namespace scbw {

struct UnitFinderResult {
  int count;                        //Number of units found.
  CUnit* units[UNIT_ARRAY_LENGTH];  //Pointer to each unit found.
};

/// Finds all units in the given bounds. This is more efficient than manually
/// looping through the entire unit table.
///
/// @param [out]  unitsFound  Is used to store the list of units found.
void findUnitsInBounds(int left, int top, int right, int bottom,
                       UnitFinderResult &unitsFound);


/// Iterates through all units within the given bounds until it finds a unit for
/// which @p isGood() returns true.
///
/// @param isGood   A callback function or functor that accepts a Unit pointer
///                 as its only argument, and returns a boolean value that
///                 indicates whether the search should be stopped.
/// @return         The unit for which isGood() returns true. If no unit passes
///                 the test, returns NULL.
template <typename callbackT>
CUnit* findFirstUnitInBounds(int left, int top, int right, int bottom,
                             callbackT &isGood) {
  UnitFinderResult unitsFound;
  findUnitsInBounds(left, top, right, bottom, unitsFound);

  for (int i = 0; i < unitsFound.count; ++i) {
    CUnit *unit = unitsFound.units[i];
    if (isGood(unit))
      return unit;
  }

  return NULL;
}

/// Iterates through all units within the given bounds, and returns the unit for
/// which @ checkScore() returns the highest value.
///
/// @param  checkScore  A callback function or functor that accepts a Unit
///                     pointer as its only argument, and returns the relative
///                     score (unsigned int) for that unit.
/// @return             The unit that receives the highest score value. If
///                     @p checkScore() returns 0 for all units, returns NULL.
template <typename callbackT>
CUnit* findBestUnitInBounds(int left, int top, int right, int bottom,
                            callbackT &checkScore) {
  UnitFinderResult unitsFound
  findUnitsInBounds(left, top, right, bottom, unitsFound);

  unsigned int bestScore = 0;
  CUnit *bestUnit = NULL;

  for (int i = 0; i < unitsFound.count; ++i) {
    CUnit *unit = unitsFound.units[i];
    unsigned int currentScore = checkScore(unit);
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestUnit = unit;
    }
  }

  return bestUnit;
}

} //scbw
