//All functions in this file are meant to be hook helpers for psi_field.h.
//Do NOT modify any functions in this file!

#include "psi_field.h"
#include "../SCBW/scbwdata.h"
#include "../tools.h"

namespace hooks {

void showAllPsiFields() {
  const u32 Func_showAllPsiFields = 0x00493640;

  __asm {
    PUSHAD
    CALL Func_showAllPsiFields
    POPAD
  }
}

void hideAllPsiFields() {
  const u32 Func_hideAllPsiFields = 0x00494100;

  __asm {
    PUSHAD
    CALL Func_hideAllPsiFields
    POPAD
  }
}

//If @p unit is a psi provider, make all psi field graphics visible.
void showAllPsiFieldsOnSelect(CUnit *unit) {
  if (!(*IS_IN_REPLAY) && unit->playerId == (*LOCAL_NATION_ID)) {
    if ((unit->status & UnitStatus::Completed) && hooks::canMakePsiField(unit)) {
       showAllPsiFields();
    }
  }
}

//If @p unit is a psi provider, make all psi field graphics invisible.
void hideAllPsiFieldsOnUnselect(CUnit *unit) {
  if (!(*IS_IN_REPLAY) && unit->playerId == (*LOCAL_NATION_ID)) {
    if ((unit->status & UnitStatus::Completed) && hooks::canMakePsiField(unit)) {
       hideAllPsiFields();
    }
  }
}

//-------- Actual hooking --------//

void __declspec(naked) showAllPsiFieldsOnSelectWrapper() {
  static CUnit *unit;
  __asm {
    PUSHAD
    MOV unit, EDI
  }

  showAllPsiFieldsOnSelect(unit);

  __asm {
    POPAD
    POP EDI
    POP ESI
    POP EBX
    MOV ESP, EBP
    POP EBP
    RETN 4
  }
}

void __declspec(naked) hideAllPsiFieldsOnUnselectWrapper() {
  static CUnit *unit;
  __asm {
    POP EDI   ;//Clean up part of the stack
    PUSHAD
    MOV unit, ESI
  }

  hideAllPsiFieldsOnUnselect(unit);

  __asm {
    POPAD
    POP ESI
    RETN
  }
}

void psiFieldHookInject() {
  jmpPatch(showAllPsiFieldsOnSelectWrapper,   0x004E6224);
  jmpPatch(hideAllPsiFieldsOnUnselectWrapper, 0x004E62BE);
}

} //hooks