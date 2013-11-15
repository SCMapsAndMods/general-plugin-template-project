#pragma once
#include "DatInterface.h"
#include <sstream>

namespace datcc {

const int FLINGY_TYPE_COUNT = 209;

#pragma pack(1)

struct FlingyDatFile {
  WORD  sprite      [FLINGY_TYPE_COUNT];
  DWORD topSpeed    [FLINGY_TYPE_COUNT];
  WORD  acceleration[FLINGY_TYPE_COUNT];
  DWORD haltDistance[FLINGY_TYPE_COUNT];
  SBYTE turnSpeed   [FLINGY_TYPE_COUNT];
  BYTE  unused      [FLINGY_TYPE_COUNT];
  BYTE  moveControl [FLINGY_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(FlingyDatFile) == 3135);

class FlingyDat {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);
  private:
    FlingyDatFile data;
};

template <class IniProcT>
void FlingyDat::processIni(IniProcT &iniProc) {
  for (int flingyId = 0; flingyId < FLINGY_TYPE_COUNT; ++flingyId) {
    std::ostringstream sectionName;
    sectionName << "Flingy #" << flingyId;
    iniProc.setSection(sectionName.str(), getFlingyName(flingyId));

    iniProc.process(data.sprite[flingyId],       "Sprite", makeSpriteComment);
    iniProc.process(data.topSpeed[flingyId],     "Top Speed", makeSpeedComment);
    iniProc.process(data.acceleration[flingyId], "Acceleration");
    iniProc.process(data.haltDistance[flingyId], "Halt Distance");
    iniProc.process(data.turnSpeed[flingyId],    "Turn Speed");
    iniProc.process(data.moveControl[flingyId],  "Move Control", makeFlingyControlTypeComment);
    iniProc.process(data.unused[flingyId],       "Unused");
  }
}

} //datcc
