#include "spider_mine.h"
#include "../hook_tools.h"

namespace {

void __declspec(naked) getSpiderMineBurrowTimeWrapper() {
  CUnit *mine;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV mine, ESI
  }

  mine->groundWeaponCooldown = hooks::getSpiderMineBurrowTimeHook(mine);
  mine->mainOrderState = 1;

  __asm {
    POPAD
    RETN
  }
}

bool __fastcall spiderMineCanTargetUnitWrapper(const CUnit* target, const CUnit* mine) {
  return hooks::spiderMineCanTargetUnitHook(target, mine);
}

} //unnamed namespace

namespace hooks {

void injectSpiderMineHooks() {
  callPatch(getSpiderMineBurrowTimeWrapper, 0x00463E08, 3);
  memoryPatch(0x0044127F, &spiderMineCanTargetUnitWrapper);
}

} //hooks
