#include "irradiate.h"
#include "../hook_tools.h"

namespace {

const u32 Func_doIrradiateDamage = 0x004555C0;

void __fastcall doIrradiateDamageWrapper(CUnit *unit) {
  hooks::doIrradiateDamageHook(unit);
}

} //unnamed namespace

namespace hooks {

void injectIrradiateHook() {
  jmpPatch(doIrradiateDamageWrapper, Func_doIrradiateDamage);
}

void doIrradiateDamage(CUnit *unit) {
  __asm {
    PUSHAD
    MOV ECX, unit
    CALL Func_doIrradiateDamage
    POPAD
  }
}

} //hooks
