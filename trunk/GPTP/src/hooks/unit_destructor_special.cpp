#include "unit_destructor_special.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include <algorithm>

//Helper function definitions. Do NOT modify!
void killAllHangarUnits(CUnit *unit);

//AKA ImageDestructor() (0x004D4CE0)
void freeImage(CImage *image) {
  if (!(screenLayers->game.hasBeenRefreshed))
    scbw::refreshScreen(image->screenPosition.x,
                        image->screenPosition.y,
                        image->screenPosition.x + image->grpSize.right,
                        image->screenPosition.y + image->grpSize.bottom
                        );

  CSprite* const parent = image->parentSprite;
  if (parent->imageHead == image)
    parent->imageHead = image->link.next;
  if (parent->imageTail == image)
    parent->imageTail = image->link.prev;

  if (image->link.prev)
    image->link.prev->link.next = image->link.next;
  if (image->link.next)
    image->link.next->link.prev = image->link.prev;

  image->link.prev = NULL;
  image->link.next = NULL;
  image->grpOffset = NULL;

  unusedImages.insertAfterHead(image);
}

//AKA SpriteDestructor() (0x00497B40)
void freeSprite(CSprite *sprite) {
  CImage *image = sprite->imageHead;
  while (image) {
    CImage *nextImage = image->link.next;
    freeImage(image);
    image = nextImage;
  }

  int y = sprite->position.y / 32;
  y = std::min(std::max(y, 0), mapTileSize->height - 1);
  if (spritesOnTile->arr1[y] == sprite)
    spritesOnTile->arr1[y] = sprite->link.prev;
  if (spritesOnTile->arr2[y] == sprite)
    spritesOnTile->arr2[y] = sprite->link.next;

  if (sprite->link.prev)
    sprite->link.prev->link.next = sprite->link.next;
  if (sprite->link.next)
    sprite->link.next->link.prev = sprite->link.prev;

  sprite->link.prev = NULL;
  sprite->link.next = NULL;

  unusedSprites.insertAfterHead(sprite);
}

void removeFromPsiProvider(CUnit *psiProvider) {
  if (psiProvider->psi_link.prev)
    psiProvider->psi_link.prev->psi_link.next = psiProvider->psi_link.next;
  if (psiProvider->psi_link.next)
    psiProvider->psi_link.next->psi_link.prev = psiProvider->psi_link.prev;

  if (psiProvider == *firstPsiFieldProvider)
    *firstPsiFieldProvider = psiProvider->psi_link.next;

  psiProvider->psi_link.prev = NULL;
  psiProvider->psi_link.next = NULL;
}

void freeResourceContainer(CUnit *resource) {
  resource->building.resource.gatherQueueCount = 0;

  CUnit *worker = resource->building.resource.nextGatherer;
  while (worker) {
    if (worker->worker.harvest_link.prev)
      worker->worker.harvest_link.prev->worker.harvest_link.next = worker->worker.harvest_link.next;
    else
      resource->building.resource.nextGatherer = worker->worker.harvest_link.next;

    if (worker->worker.harvest_link.next)
      worker->worker.harvest_link.next->worker.harvest_link.prev = worker->worker.harvest_link.prev;

    CUnit *nextWorker = worker->worker.harvest_link.next;
    worker->worker.harvestTarget = NULL;
    worker->worker.harvest_link.prev = NULL;
    worker->worker.harvest_link.next = NULL;
    worker = nextWorker;
  }
}


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
    freeResourceContainer(unit);
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
    if (unit->building.pylonAura) {
      freeSprite(unit->building.pylonAura);
      unit->building.pylonAura = NULL;
    }
    removeFromPsiProvider(unit);
    *CAN_UPDATE_POWERED_STATUS = 1;
    return;
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
