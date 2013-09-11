/// Support for adding/removing psi fields. To use, you will also have to modify
/// the following hooks:
///
///   hooks/unit_destructor_special.cpp
///   hooks/unit_selection_graphics.cpp

#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

/// Used by psi_field_select.cpp. You can edit this function in psi_field.cpp.
bool canMakePsiField(CUnit *unit);

/// Used by unit_destructor_sepcial.cpp. You can edit this function in psi_field.cpp.
bool canMakePsiField(u16 unitId);

/// This function must be called once per each unit in nextFrame().
void updatePsiFieldProvider(CUnit *unit);

/// Used in multiple hooks. Do NOT call this in game_hooks.cpp!
void removePsiField(CUnit *unit);

//Related to unit selection. Call this in initialize.h.
void psiFieldHookInject();

} //hooks
