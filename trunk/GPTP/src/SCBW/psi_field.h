/// Support for adding/removing psi fields. To use, you will also have to modify
/// the following hooks:
///
///   hooks/unit_destructor_special.cpp
///   hooks/?

#pragma once
#include "structures/CUnit.h"

namespace scbw {

/// Adds @p unit to the psi provider list and creates the psi field sprite.
/// If the unit is already a psi provider, this function does nothing.
void addPsiField(CUnit *unit);

/// Removes @p unit from the psi provider list and destroys the psi field sprite.
void removePsiField(CUnit *unit);

} //scbw
