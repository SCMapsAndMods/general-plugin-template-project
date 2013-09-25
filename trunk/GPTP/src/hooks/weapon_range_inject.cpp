#include "weapon_range.h"
#include "../hook_tools.h"

namespace {

//Inject with jmpPatch()
const u32 Hook_GetSeekRange       = 0x00476000;
static void __declspec(naked) getSeekRangeWrapper() {
  CUnit *unit;
  u8 seekRange;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDX
  }

  seekRange = hooks::getSeekRangeHook(unit);

  __asm {
    MOVZX EAX, seekRange
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}

//Inject with jmpPatch()
const u32 Hook_GetMaxWeaponRange  = 0x00475870;
static void __declspec(naked) getMaxWeaponRangeWrapper() {
  CUnit *unit;
  u8 weaponId;
  u32 maxWeaponRange;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
    MOV weaponId, BL
  }

  maxWeaponRange = hooks::getMaxWeaponRangeHook(unit, weaponId);

  __asm {
    MOV EAX, maxWeaponRange;
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectWeaponRangeHooks() {
  jmpPatch(getSeekRangeWrapper,       Hook_GetSeekRange);
  jmpPatch(getMaxWeaponRangeWrapper,  Hook_GetMaxWeaponRange);
}

} //hooks
