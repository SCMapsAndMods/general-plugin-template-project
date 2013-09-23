#include "harvest.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

namespace hooks {

//Identical to function @ 0x00468830
void updateMineralPatchImage(CUnit *mineralPatch) {
  IscriptAnimation::Enum anim;

  if (mineralPatch->building.resource.resourceAmount >= 750)
    anim = IscriptAnimation::WalkingToIdle;
  else if (mineralPatch->building.resource.resourceAmount >= 500)
    anim = IscriptAnimation::AlmostBuilt;
  else if (mineralPatch->building.resource.resourceAmount >= 250)
    anim = IscriptAnimation::SpecialState2;
  else
    anim = IscriptAnimation::SpecialState1;

  if (anim != mineralPatch->building.resource.resourceIscript) {
    mineralPatch->building.resource.resourceIscript = anim;
    mineralPatch->sprite->playIscriptAnim(anim);
  }
}

//Harvests minerals/gas from the @p resource and returns the amount that a
//worker should carry.
u8 harvestResourceFrom(CUnit *resource, bool isMineral) {
  //Default StarCraft behavior

  if (resource->building.resource.resourceAmount < 8) {
    if (isMineral) {
      resource->remove();
      return (u8) resource->building.resource.resourceAmount;
    }
    else {
      resource->building.resource.resourceAmount = 0;
      return 2;
    }
  }
  else {
    resource->building.resource.resourceAmount -= 8;
    
    if (isMineral) {
      if (resource->building.resource.resourceAmount > 0)
        updateMineralPatchImage(resource);
      else
        resource->remove();
    }
    else if (resource->building.resource.resourceAmount < 8)
      scbw::showErrorMessageWithSfx(resource->playerId, 875, 20);
    
    return 8;
  }
}

void updateImagePositionOffset(CImage *image) {
  const u32 Func_updateImagePositionOffset = 0x004D5A00;
  __asm {
    PUSHAD
    MOV ECX, image
    CALL Func_updateImagePositionOffset
    POPAD
  }
}

//Identical to function @ 0x004F3AF0
void setResourceAmountCarried(CUnit *worker, u8 amountCarried, u32 chunkImageId, bool isMineral) {
  if (worker->resourceType) return;
  worker->resourceType = isMineral ? 2 : 1;
  
  CImage *chunkImage = worker->sprite->createOverlay(chunkImageId);
  if (chunkImage && !(chunkImage->flags & 0x80)) {
    chunkImage->flags |= 0x80;
    updateImagePositionOffset(chunkImage);
  }

  worker->worker.resourceCarryAmount = amountCarried;
  scbw::refreshButtonSet();
}

void transferResourceToWorker(CUnit *worker, CUnit *resource) {
  //Default StarCraft behavior

  u32 chunkImageId;
  bool isMineral = false;

  if (176 <= resource->id && resource->id <= 178) {  //Is a mineral patch
    chunkImageId = 397; //Mineral Chunk Type 1
    isMineral = true;
  }
  else if (resource->id == UnitId::assimilator)
    chunkImageId = 399; //Protoss Gas Orb Type 1
  else if (resource->id == UnitId::extractor)
    chunkImageId = 401; //Zerg Gas Sac Type 1
  else if (resource->id == UnitId::refinery)
    chunkImageId = 403; //Terran Gas Tank Type 1
  else
    return;

  u8 resourceAmount = harvestResourceFrom(resource, isMineral);
  if (resourceAmount < 8)
    chunkImageId += 1;  //Use depleted (smaller) chunk image

  if (resourceAmount > 0) {
    if (worker->resourceType & 3) { //Is carrying a mineral / gas
      //Identical to function @ 0x004F3900
      //Remove powerup shadows (mineral chunk shadow, psi emitter shadow, etc.)
      worker->sprite->removeOverlay(405, 418);
      //Remove powerup graphics
      worker->sprite->removeOverlay(392, 404);
      //Remove Uraj / Khalis
      worker->sprite->removeOverlay(958, 959);
      worker->resourceType = 0;
    }

    setResourceAmountCarried(worker, resourceAmount, chunkImageId, isMineral);
  }
}

} //hooks
