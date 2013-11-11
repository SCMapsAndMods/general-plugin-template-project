#include "consume.h"
#include "../hook_tools.h"

//Inject with jmpPatch
void __declspec(naked) unitIsConsumableWrapper() {
	CUnit* unit;
	u8 defilerOwner;
	__asm {
		PUSHAD
		MOV EBP, ESP
		MOV unit, EAX
		MOV defilerOwner, BL
	}

  if (hooks::unitIsConsumable(unit, defilerOwner)) {
		__asm {
			POPAD
			XOR AX, AX	;//Equivalent to MOV AX, 0
			POP EBP
			RETN 4
		}
	}
	else {
		__asm {
			POPAD
			MOV AX, 897	;//stat_txt.tbl string: Invalid target.<0>
			POP EBP
			RETN 4
		}
	}
}

//Inject with jmpPatch()
void __declspec(naked) onConsumeUnitWrapper() {
	CUnit *caster, *target;

	__asm {
		PUSHAD
		MOV EBP, ESP
		MOV caster, ESI
		MOV target, EDI
	}

  hooks::onConsumeUnit(caster, target);

	__asm {
	  POPAD
	  POP EBX
	  POP EDI
	  RETN
	}
}

namespace hooks {

void injectConsumeHooks() {
  jmpPatch(unitIsConsumableWrapper, 0x00491F51);
  jmpPatch(onConsumeUnitWrapper,    0x004F47DE);
}

} //hooks