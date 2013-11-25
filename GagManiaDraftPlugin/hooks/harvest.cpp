#include "harvest.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

//Helper functions
void updateMineralPatchImage(CUnit *mineralPatch);
void setResourceAmountCarried(CUnit *worker, u8 amountCarried, u32 chunkImageId, bool isMineral);

//-------- Actual hooks --------//

namespace hooks {

//Harvests minerals/gas from the @p resource and returns the amount that a
//worker should carry.
u8 harvestResourceFrom(CUnit *resource, CUnit *worker, bool isMineral) {

  //미네랄은 4, 가스는 6만큼 채취
  u8 harvestAmount = isMineral ? 4 : 6;
  u8 workerPlayerRace = playerTable[worker->playerId].race;

  //미네랄 채취
  if (isMineral) {
    if (resource->building.resource.resourceAmount <= harvestAmount) {
      resource->remove();
      
      //미네랄 고갈 메시지 재생
      if (workerPlayerRace == PlayerRace::Zerg)
        scbw::showErrorMessageWithSfx(worker->playerId, 1701, 59);
      else if (workerPlayerRace == PlayerRace::Terran)
        scbw::showErrorMessageWithSfx(worker->playerId, 1701, 60);
      else if (workerPlayerRace == PlayerRace::Protoss)
        scbw::showErrorMessageWithSfx(worker->playerId, 1701, 61);

      return (u8) resource->building.resource.resourceAmount;
    }
    else {
      resource->building.resource.resourceAmount -= harvestAmount;
      updateMineralPatchImage(resource);
    }
  }
  //가스 채취
  else {
    if (resource->building.resource.resourceAmount < harvestAmount) {
      resource->building.resource.resourceAmount = 0;
      return 3; //고갈된 가스 채취량
    }
    else {
      resource->building.resource.resourceAmount -= harvestAmount;
      //다음 채취할 때 고갈될 예정
      if (resource->building.resource.resourceAmount <= harvestAmount) {
        //가스 고갈 메시지 재생
        if (workerPlayerRace == PlayerRace::Zerg)
          scbw::showErrorMessageWithSfx(worker->playerId, 875, 20);
        else if (workerPlayerRace == PlayerRace::Terran)
          scbw::showErrorMessageWithSfx(worker->playerId, 875, 11);
        else if (workerPlayerRace == PlayerRace::Protoss)
          scbw::showErrorMessageWithSfx(worker->playerId, 875, 12);
      }
    }
  }

  return harvestAmount;
}

//Transfers a set amount of resources from a resource patch to a worker.
void transferResourceToWorkerHook(CUnit *worker, CUnit *resource) {

  u32 chunkImageId;
  bool isMineral = false;

  if (176 <= resource->id && resource->id <= 178) {  //Is a mineral patch
    chunkImageId = ImageId::MineralChunkType1;
    isMineral = true;
  }
  else if (resource->id == UnitId::assimilator)
    chunkImageId = ImageId::ProtossGasOrbType1;
  else if (resource->id == UnitId::extractor)
    chunkImageId = ImageId::ZergGasSacType1;
  else if (resource->id == UnitId::refinery)
    chunkImageId = ImageId::TerranGasTankType1;
  else
    return;

  u8 defaultHarvestAmount = isMineral ? 4 : 6;
  u8 resourceAmount = harvestResourceFrom(resource, worker, isMineral);
  if (resourceAmount < defaultHarvestAmount)
    chunkImageId += 1;  //Use depleted (smaller) chunk image

  if (resourceAmount > 0) {
    if (worker->resourceType & 3) { //Is carrying a mineral / gas
      //Identical to function @ 0x004F3900
      //Remove powerup shadows (mineral chunk shadow, psi emitter shadow, etc.)
      worker->sprite->removeOverlay(ImageId::MineralChunkShadow, ImageId::PsiEmitterShadow_Carried);
      //Remove powerup graphics
      worker->sprite->removeOverlay(ImageId::Flag, ImageId::TerranGasTankType2);
      //Remove Uraj / Khalis
      worker->sprite->removeOverlay(ImageId::Uraj, ImageId::Khalis);
      worker->resourceType = 0;
    }

    setResourceAmountCarried(worker, resourceAmount, chunkImageId, isMineral);
  }
}

} //hooks

//-------- Helper function definitions. Do NOT change this! --------//

//Identical to function @ 0x00468830
void updateMineralPatchImage(CUnit *mineralPatch) {
  IscriptAnimation::Enum anim;

  if (mineralPatch->building.resource.resourceAmount >= 750)
    anim = IscriptAnimation::WorkingToIdle;
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
  scbw::refreshConsole();
}
