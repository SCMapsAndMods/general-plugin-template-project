#include "update_unit_timers.h"
#include "../hook_tools.h"

namespace {

//Inject with jmpPatch()
const u32 Hook_UpdateUnitTimers = 0x004EC290;
void __declspec(naked) updateUnitTimersWrapper() {
  CUnit *unit;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  hooks::updateUnitTimersHook(unit);

  __asm {
    POPAD
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectUpdateUnitTimers() {
  jmpPatch(updateUnitTimersWrapper, Hook_UpdateUnitTimers);
}

} //hooks
