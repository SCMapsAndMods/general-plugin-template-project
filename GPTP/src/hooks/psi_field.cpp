#include "psi_field.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/api.h"

namespace hooks {

//Unit type check (used in unit destructor)
bool canMakePsiField(u16 unitId) {
  if (unitId == UnitId::pylon || unitId == UnitId::shuttle)
    return true;
  return false;
}

//Actual check whether a unit can build
bool canMakePsiField(CUnit *unit) {
  //NOT default StarCraft behavior

  if (unit->id == UnitId::pylon)
    return true;

  if (unit->id == UnitId::shuttle) {
    for (int i = 0; i < 8; ++i) {
      if (unit->loadedUnitIndex[i] == 0) continue;
      const int unitIndex = unit->loadedUnitIndex[i] % 2048 - 1;
      const CUnit *loadedUnit = &unitTable[unitIndex];

      if (loadedUnit->mainOrderId != OrderId::Die
          && loadedUnit->id == UnitId::probe) {
        return true;
      }
    }
  }

  return false;
}


//-------- Hook related. Do NOT change anything below this! --------//

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

//Adds @p unit to the psi provider list and creates the psi field sprite.
//If the unit is already a psi provider, this function does nothing.
//Based on orders_InitPsiProvider() @ 00493F70
//But this does not affect the unit's current order.
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

//Removes @p unit from the psi provider list and destroys the psi field sprite.
void removePsiField(CUnit *unit) {
  if (unit->building.pylonAura) {
    unit->building.pylonAura->free();
    unit->building.pylonAura = NULL;
  }
  removeFromPsiProviderList(unit);
  *canUpdatePoweredStatus = true;
}

//-------- Update psi field position --------//

void refreshSpriteData(CSprite *sprite) {
  const u32 Func_refreshSpriteData = 0x004983A0;
  __asm {
    PUSHAD
    MOV EAX, sprite
    CALL Func_refreshSpriteData
    POPAD
  }
}

//Synchronizes the unit's psi field sprite position with the unit.
void updatePsiFieldPosition(const CUnit *unit) {
  if (!unit || !unit->sprite || !unit->building.pylonAura) return;

  CSprite *psiField = unit->building.pylonAura;
  if (unit->sprite->position != psiField->position) {
    //for (CImage *i = psiField->imageHead; i; i = i->link.next)
    //  i->flags |= 1;  //Redraw image

    psiField->setPosition(unit->getX(), unit->getY());
    refreshSpriteData(psiField);
    *canUpdatePoweredStatus = true;
  }
}

//-------- Update psi field provider --------//

void updatePsiFieldProvider(CUnit *unit) {
  if (!unit || unit->mainOrderId == OrderId::Die) return;

  //Exclude unit types that cannot make psi fields, including: workers,
  //factories, resource containers, ghosts, nydus canals, silos, hatcheries, and powerups
  if (Unit::BaseProperty[unit->id] & (UnitProperty::Worker | UnitProperty::ResourceContainer | UnitProperty::NPCOrAccessories)
      || Unit::GroupFlags[unit->id].isFactory
      || unit->id == UnitId::ghost
      || unit->id == UnitId::sarah_kerrigan
      || unit->id == UnitId::Hero_AlexeiStukov
      || unit->id == UnitId::Hero_SamirDuran
      || unit->id == UnitId::Hero_InfestedDuran
      || unit->id == UnitId::nydus_canal
      || unit->id == UnitId::nuclear_silo
      || unit->id == UnitId::hatchery 
      || unit->id == UnitId::lair
      || unit->id == UnitId::hive)
    return;


  if (unit->status & UnitStatus::Completed) {
    if (canMakePsiField(unit)) {
      addPsiField(unit);
      updatePsiFieldPosition(unit);
    }
    else
      removePsiField(unit);
  }
}


} //hooks
