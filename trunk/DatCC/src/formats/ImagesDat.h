#pragma once
#include "DatInterface.h"
#include <sstream>

namespace datcc {

//Based on http://modcrafters.com/wiki/index.php?title=Images.dat

const int IMAGE_TYPE_COUNT = 999;

#pragma pack(1)

struct ImagesDatFile {
  DWORD grpFile         [IMAGE_TYPE_COUNT];
  BYTE  isTurnable      [IMAGE_TYPE_COUNT];
  BYTE  isClickable     [IMAGE_TYPE_COUNT];
  BYTE  useFullIscript  [IMAGE_TYPE_COUNT];
  BYTE  drawIfCloaked   [IMAGE_TYPE_COUNT];
  BYTE  drawFunction    [IMAGE_TYPE_COUNT];
  BYTE  remapping       [IMAGE_TYPE_COUNT];
  DWORD iscriptEntry    [IMAGE_TYPE_COUNT];
  DWORD shieldOverlayLO [IMAGE_TYPE_COUNT];
  DWORD attackOverlayLO [IMAGE_TYPE_COUNT];
  DWORD injuryOverlayLO [IMAGE_TYPE_COUNT];
  DWORD specialOverlayLO[IMAGE_TYPE_COUNT];
  DWORD landingDustLO   [IMAGE_TYPE_COUNT];
  DWORD liftOffDustLO   [IMAGE_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(ImagesDatFile) == 37962);


class ImagesDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    ImagesDatFile data;
};

template <class IniProcT>
void ImagesDat::processIni(IniProcT &iniProc) {
  for (int imageId = 0; imageId < IMAGE_TYPE_COUNT; ++imageId) {
    std::ostringstream sectionName;
    sectionName << "Image #" << imageId;
    iniProc.setSection(sectionName.str(), getImageName(imageId));

    iniProc.process(data.grpFile[imageId],          "GRP File", makeImagesTblComment);
    iniProc.process(data.isTurnable[imageId],       "Is Turnable");
    iniProc.process(data.isClickable[imageId],      "Is Clickable");
    iniProc.process(data.useFullIscript[imageId],   "Use Full Iscript");
    iniProc.process(data.drawIfCloaked[imageId],    "Draw If Cloaked");
    iniProc.process(data.drawFunction[imageId],     "Drawing Function", makeDrawingFunctionComment);
    iniProc.process(data.remapping[imageId],        "Remapping", makeRemappingComment);
    iniProc.process(data.iscriptEntry[imageId],     "Iscript Entry");
    
    iniProc.process(data.shieldOverlayLO[imageId],  "Shield Overlay LO File", makeImagesTblComment);
    iniProc.process(data.attackOverlayLO[imageId],  "Attack Overlay LO File", makeImagesTblComment);
    iniProc.process(data.injuryOverlayLO[imageId],  "Injury Overlay LO File", makeImagesTblComment);
    iniProc.process(data.specialOverlayLO[imageId], "Special Overlay LO File", makeImagesTblComment);
    iniProc.process(data.landingDustLO[imageId],    "Landing Dust LO File", makeImagesTblComment);
    iniProc.process(data.liftOffDustLO[imageId],    "Lift-Off Dust LO File", makeImagesTblComment);
  }
}

} //datcc
