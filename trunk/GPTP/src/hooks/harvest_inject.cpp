#include "harvest.h"
#include "../tools.h"

namespace hooks {

void __declspec(naked) harvestResourceWrapper() {
  static CUnit *resource;
  static u8 isMineral;
  static u8 resourceAmountReturned;
  __asm {
    PUSHAD
    MOV resource, EAX
    MOV isMineral, CL
  }

  resourceAmountReturned = harvestResourceHook(resource, isMineral != 0);

  __asm {
    POPAD
    MOV AL, resourceAmountReturned
    RETN
  }
}

void harvestResourceInject() {
  callPatch(harvestResourceWrapper, 0x00469713);
}


} //hooks
