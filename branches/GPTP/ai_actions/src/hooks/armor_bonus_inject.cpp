#include "armor_bonus.h"
#include "../hook_tools.h"

namespace {

//Inject with jmpPatch()
const u32 Hook_GetArmorBonus  = 0x00453FC0;
void __declspec(naked) getArmorBonusWrapper() {
  CUnit *unit;
  u8 armor;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  armor = hooks::getArmorBonusHook(unit);

  __asm {
    MOVZX EAX, armor
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectArmorBonusHook() {
  jmpPatch(getArmorBonusWrapper, Hook_GetArmorBonus);
}

} //hooks
