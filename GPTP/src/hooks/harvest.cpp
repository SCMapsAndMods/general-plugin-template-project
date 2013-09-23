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

//Function @ 0x00469650
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

} //hooks
