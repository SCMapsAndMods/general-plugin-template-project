#include "DatInterface.h"
#include <sstream>

namespace datcc {

//Based on http://modcrafters.com/wiki/index.php?title=Sprites.dat

const int SPRITE_TYPE_COUNT   = 517;
const int SPRITE_SELECTABLES  = 387;  //IDs 130-516

#pragma pack(1)

struct SpritesDatFile {
  WORD  image               [SPRITE_TYPE_COUNT];
  BYTE  healthBarSize       [SPRITE_SELECTABLES];
  BYTE  unknown             [SPRITE_TYPE_COUNT];
  BYTE  isVisible           [SPRITE_TYPE_COUNT];
  BYTE  selectionCircle     [SPRITE_SELECTABLES];
  SBYTE selectionCircleVPos [SPRITE_SELECTABLES];
};

#pragma pack()

C_ASSERT(sizeof(SpritesDatFile) == 3229);


class SpritesDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    SpritesDatFile data;
};

template <class IniProcT>
void SpritesDat::processIni(IniProcT &iniProc) {
  for (int spriteId = 0; spriteId < SPRITE_TYPE_COUNT; ++spriteId) {
    std::ostringstream sectionName;
    sectionName << "Sprite #" << spriteId;
    iniProc.setSection(sectionName.str(), getSpriteName(spriteId));

    iniProc.processImageId(data.image[spriteId], "Image");
    iniProc.process(data.isVisible[spriteId], "IsVisible");
    iniProc.process(data.unknown[spriteId], "Unknown");

    //Selectable sprites
    if (130 <= spriteId && spriteId <= 516) {
      iniProc.process(data.healthBarSize[spriteId - 130], "HP Bar Size");

      //@TODO Fix this to use a separate hint comment
      WORD selectionCircle = data.selectionCircle[spriteId - 130] + 561;
      iniProc.processImageId(selectionCircle, "Selection Circle");
      data.selectionCircle[spriteId - 130] = selectionCircle - 561;

      iniProc.process(data.selectionCircleVPos[spriteId - 130], "Selection Circle V-Offset");
    }
  }
}

} //datcc
