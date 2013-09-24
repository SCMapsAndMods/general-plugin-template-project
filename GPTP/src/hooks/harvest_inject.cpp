#include "harvest.h"
#include "../hook_tools.h"

namespace hooks {

void __declspec(naked) transferResourceToWorkerWrapper() {
  CUnit *worker, *resource;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV worker, ECX
    MOV resource, EAX
  }

  transferResourceToWorkerHook(worker, resource);

  __asm {
    POPAD
    RETN
  }
}

void harvestResourceInject() {
  jmpPatch(transferResourceToWorkerWrapper, 0x004696D0);
}


} //hooks
