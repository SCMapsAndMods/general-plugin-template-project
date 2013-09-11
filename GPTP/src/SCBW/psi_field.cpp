#include "psi_field.h"
#include "scbwdata.h"

namespace scbw {

//-------- Add psi field --------//

//AKA createPsiFieldSpriteForUnit()
//Returns true if successful, false otherwise.
bool addPsiFieldSprite(CUnit *unit) {
  if (!unit) return false;

  const u32 Func_addPsiFieldSprite = 0x00493BF0;
  static u32 result;
  __asm {
    PUSHAD
    MOV ECX, unit
    CALL Func_addPsiFieldSprite
    MOV result, EAX
    POPAD
  }

  return result != 0;
}

//Based on orders_InitPsiProvider() @ 00493F70
//But this does not affect the unit's current order
void addPsiField(CUnit *unit) {
  if (!unit) return;

  if (*firstPsiFieldProvider == unit || unit->psi_link.next || unit->psi_link.prev)
    return;

  unit->psi_link.prev = NULL;
  unit->psi_link.next = *firstPsiFieldProvider;
  if (*firstPsiFieldProvider)
    (*firstPsiFieldProvider)->psi_link.prev = unit;
  *firstPsiFieldProvider = unit;
  addPsiFieldSprite(unit);
  *canUpdatePoweredStatus = true;
}


//-------- Remove psi field --------//

//Identical to function @ 0x00493100
void removeFromPsiProviderList(CUnit *psiProvider) {
  if (psiProvider->psi_link.prev)
    psiProvider->psi_link.prev->psi_link.next = psiProvider->psi_link.next;
  if (psiProvider->psi_link.next)
    psiProvider->psi_link.next->psi_link.prev = psiProvider->psi_link.prev;

  if (psiProvider == *firstPsiFieldProvider)
    *firstPsiFieldProvider = psiProvider->psi_link.next;

  psiProvider->psi_link.prev = NULL;
  psiProvider->psi_link.next = NULL;
}

void removePsiField(CUnit *unit) {
  if (unit->building.pylonAura) {
    unit->building.pylonAura->free();
    unit->building.pylonAura = NULL;
  }
  removeFromPsiProviderList(unit);
}

} //scbw
