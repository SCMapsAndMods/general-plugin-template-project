#include "unit_destructor_special.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

//Helper function definitions. Do NOT modify!
void killAllHangarUnits(CUnit *unit);


void unitDestructorSpecialHook(CUnit *unit) {
  //Destroy interceptors and scarabs
  if (unit->id == UnitId::carrier || unit->id == UnitId::gantrithor
      || unit->id == UnitId::reaver || unit->id == UnitId::warbringer
      ) {
    killAllHangarUnits(unit);
    return;
  }
  
  //Destroy nuclear missiles mid-launch
  if (unit->id == UnitId::ghost
      || unit->id == UnitId::sarah_kerrigan
      || unit->id == UnitId::Hero_AlexeiStukov
      || unit->id == UnitId::Hero_SamirDuran
      || unit->id == UnitId::Hero_InfestedDuran
      ) {
     if (unit->building.ghost.nukeMissile) {
       unit->building.ghost.nukeMissile->playIscriptAnim(IscriptAnimation::Death);
       unit->building.ghost.nukeMissile = NULL;
     }
     return;
  }

  //Is a harvestable mineral patch or gas building
  if (UnitId::ResourceMineralField <= unit->id && unit->id <= UnitId::ResourceMineralFieldType3
      || (unit->id == UnitId::refinery || unit->id == UnitId::assimilator || unit->id == UnitId::extractor)
         && unit->status & UnitStatus::Completed
      ) {
    //TODO: add content
    return;
  }

  //Is a scarab or interceptor
  if (unit->id == UnitId::ProtossScarab || unit->id == UnitId::interceptor) {
    if (unit->status & UnitStatus::Completed) {
      if (unit->interceptor.parent) {
        if (unit->interceptor.next)
          unit->interceptor.next->interceptor.prev = unit->interceptor.prev;
        else {
          if (unit->interceptor.isOutsideHangar)
            unit->interceptor.parent->carrier.outHangarChild = unit->interceptor.prev;
          else
            unit->interceptor.parent->carrier.inHangarChild = unit->interceptor.prev;
        }
        
        if (unit->interceptor.isOutsideHangar)
          unit->interceptor.parent->carrier.outHangarCount--;
        else
          unit->interceptor.parent->carrier.inHangarCount--;

        if (unit->interceptor.prev)
          unit->interceptor.prev->interceptor.next = unit->interceptor.next;
      }
      else {
        unit->interceptor.prev = NULL;
        unit->interceptor.next = NULL;
      }
    }
    return;
  }

  //Is a Nuclear Silo
  if (unit->id == UnitId::nuclear_silo) {
    if (unit->building.silo.nuke) {
      unit->building.silo.nuke->remove();
      unit->building.silo.nuke = NULL;
    }
    return;
  }

  //Is a Nuclear Missile
  if (unit->id == UnitId::nuclear_missile) {
    if (unit->connectedUnit && unit->connectedUnit->id == UnitId::nuclear_silo) {
      unit->connectedUnit->building.silo.nuke = NULL;
      unit->connectedUnit->building.silo.hasNuke = 0;
    }
    return;
  }

  if (unit->id == UnitId::pylon) {
    //TODO: fill
  }

  if (unit->id == UnitId::nydus_canal) {
    CUnit *nydusExit = unit->building.nydus.exit;
    if (nydusExit) {
      unit->building.nydus.exit = NULL;
      nydusExit->building.nydus.exit = NULL;
      nydusExit->remove();
    }
    return;
  }
  
}



/**** Helper function definitions. Do not change anything below this! ****/

void killAllHangarUnits(CUnit *unit) {
  while (unit->carrier.inHangarCount--) {
    CUnit *childInside = unit->carrier.inHangarChild;
    unit->carrier.inHangarChild = childInside->interceptor.prev;
    childInside->interceptor.parent = NULL;
    childInside->remove();
  }

  while (unit->carrier.outHangarCount--) {
    CUnit *childOutside = unit->carrier.outHangarChild;
    unit->carrier.outHangarChild = childOutside->interceptor.prev;
    childOutside->interceptor.parent = NULL;
    childOutside->interceptor.prev = NULL;
    childOutside->interceptor.next = NULL;

    //Kill interceptors only (Scarabs will defuse anyway)
    if (childOutside->id != UnitId::ProtossScarab) {
      const u16 deathTimer = scbw::randBetween(15, 45);
      if (childOutside->removeTimer == 0
          || childOutside->removeTimer > deathTimer)
        childOutside->removeTimer = deathTimer;
    }
  }

  unit->carrier.outHangarChild = NULL;
}
