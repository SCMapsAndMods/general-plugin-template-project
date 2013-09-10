#include "../SCBW/structures.h"
#include "../tools.h"

void unitDestructorSpecialHook(CUnit *unit);

static void __declspec(naked) unitDestructorSpecialWrapper() {
  CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  unitDestructorSpecialHook(unit);

  __asm {
    POPAD
    RETN
  }
}

static void unitDestructorSpecialInject() {
  callPatch(unitDestructorSpecialWrapper, 0x004A075F);
}
