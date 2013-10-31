#include "building_morph.h"
#include <hook_tools.h>
#include <SCBW/api.h>
#include <algorithm>

/*
Need to inject:
0043352E (AI-related)
0043507F (AI-related)
*/


namespace {

//-------- isMorphingBuilding() --------//

void __declspec(naked) isMorphingBuildingWrapper() {
  static CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  if (!(unit->status & UnitStatus::Completed)
    && hooks::isMorphedBuildingHook(unit->buildQueue[unit->buildQueueSlot]))
  {
    __asm {
      POPAD
      MOV EAX, 1
      RETN
    }
  }
  else {
    __asm {
      POPAD
      XOR EAX, EAX  ;//Set EAX to 0
      RETN
    }
  }
}

//-------- UnitStatAct_ProgressBarIconText --------//

const u32 Hook_IsMorphedBuilding_ConsoleProgressBarTextBack = 0x00426B74;
void __declspec(naked) isMorphedBuildingWrapper_ConsoleProgressBarText() {
  static const CUnit *unit;
  static u16 tblErrorMessage;
  __asm {
    PUSHAD
    MOV EBP, ESP
  }

  unit = *activePortraitUnit;
  if (unit->status & UnitStatus::GroundedBuilding && unit->isFrozen())
  {
    tblErrorMessage = 828;    //<6>Disabled<0>
  }
  else {
    u16 unitId = unit->buildQueue[unit->buildQueueSlot];
    if (unit->status & UnitStatus::Completed
        || !hooks::isMorphedBuildingHook(unitId))
      unitId = unit->id;

    if (unitId == UnitId::gateway || unitId == UnitId::stargate)
      tblErrorMessage = 771;  //Morphing<0>, Building<0>, Opening Warp Gate<0>
    else
      tblErrorMessage = 774;  //Morphing<0>, Building<0>, Building<0>
  }

  __asm {
    POPAD
    MOV ECX, unit
    MOVZX EDX, tblErrorMessage
    JMP Hook_IsMorphedBuilding_ConsoleProgressBarTextBack
  }
}

//-------- statdata_UnitWireframeUpdate() --------//

const u32 Hook_IsMorphedBuilding_ConsoleWireframeUpdateBack = 0x00456955;
void __declspec(naked) isMorphedBuildingWrapper_ConsoleWireFrameUpdate() {
  static CUnit *unit;
  static u16 buildUnitId;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  buildUnitId = unit->buildQueue[unit->buildQueueSlot];
  if (unit->status & UnitStatus::Completed
      || !hooks::isMorphedBuildingHook(buildUnitId))
    buildUnitId = unit->id;

  __asm {
    POPAD
    MOVZX ECX, buildUnitId
    JMP Hook_IsMorphedBuilding_ConsoleWireframeUpdateBack
  }
}

//-------- orders_ZergBuildSelf() --------//

const u32 Hook_isMorphedBuilding_ZergBuildSelf_GetTypeBack = 0x0045D53F;
void __declspec(naked) isMorphedBuildingWrapper_ZergBuildSelf_GetType() {
  static CUnit *unit;
  static u16 buildUnitId;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDI
  }

  buildUnitId = unit->buildQueue[unit->buildQueueSlot];
  if (!hooks::isMorphedBuildingHook(buildUnitId))
    buildUnitId = unit->id;

  __asm {
    POPAD
    MOVZX EBX, buildUnitId
    JMP Hook_isMorphedBuilding_ZergBuildSelf_GetTypeBack
  }
}

void __declspec(naked) isMorphedBuildingWrapper_ZergBuildSelf_Complete() {
  static u16 unitId;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unitId, AX
  }

  if (hooks::isMorphedBuildingHook(unitId)) {
    __asm {
      POPAD
      MOV EAX, 1
      RETN
    }
  }
  else {
    __asm {
      POPAD
      XOR EAX, EAX  ;//Set EAX to 0
      RETN
    }
  }
}

//This was moved out of the naked function to prevent stack corruption bug
void zergBuildSelf_UpdateBuildTimerAndHp(CUnit *unit, u16 unitId) {
  bool isOperationCwalEnabled = scbw::isCheatEnabled(CheatFlags::OperationCwal);
  if (unit->remainingBuildTime) {
    if (isOperationCwalEnabled)
      unit->remainingBuildTime -= std::min<u16>(unit->remainingBuildTime, 16);
    else
      unit->remainingBuildTime--;
  }
  if (!hooks::isMorphedBuildingHook(unitId)) {
    u32 hpGain = unit->buildRepairHPGain;
    if (isOperationCwalEnabled)
      hpGain *= 16;
    unit->setHp(unit->hitPoints + hpGain);
  }
}

void __declspec(naked) isMorphedBuildingWrapper_ZergBuildSelf_SetTimer() {
  static CUnit *unit;
  static u16 unitId;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDI
    MOV unitId, BX
  }

  zergBuildSelf_UpdateBuildTimerAndHp(unit, unitId);

  __asm {
    POPAD
    POP EDI
    POP ESI
    POP EBX
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectBuildingMorphHooks() {
  jmpPatch(isMorphingBuildingWrapper, 0x0045CD00);
  jmpPatch(isMorphedBuildingWrapper_ConsoleProgressBarText, 0x00426AA2);
  jmpPatch(isMorphedBuildingWrapper_ConsoleWireFrameUpdate, 0x00456911);
  jmpPatch(isMorphedBuildingWrapper_ZergBuildSelf_GetType,  0x0045D505);
  callPatch(isMorphedBuildingWrapper_ZergBuildSelf_Complete, 0x0045D65D);
  jmpPatch(isMorphedBuildingWrapper_ZergBuildSelf_SetTimer, 0x0045D56C);
}

} //hooks
