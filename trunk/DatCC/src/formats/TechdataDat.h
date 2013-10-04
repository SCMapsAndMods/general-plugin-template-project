#pragma once
#include "DatInterface.h"
#include <sstream>

namespace datcc {

//Based on http://modcrafters.com/wiki/index.php?title=Techdata.dat

const int TECH_TYPE_COUNT = 44;

#pragma pack(1)

struct TechdataDatFile {
  WORD  mineralCost     [TECH_TYPE_COUNT];
  WORD  gasCost         [TECH_TYPE_COUNT];
  WORD  researchTime    [TECH_TYPE_COUNT];
  WORD  energyCost      [TECH_TYPE_COUNT];
  DWORD unknown         [TECH_TYPE_COUNT];
  WORD  icon            [TECH_TYPE_COUNT];
  SWORD label           [TECH_TYPE_COUNT];
  BYTE  race            [TECH_TYPE_COUNT];
  BYTE  unused          [TECH_TYPE_COUNT];
  BYTE  isBroodWarOnly  [TECH_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(TechdataDatFile) == 836);


class TechdataDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    TechdataDatFile data;
};

template <class IniProcT>
void TechdataDat::processIni(IniProcT &iniProc) {
  for (int techId = 0; techId < TECH_TYPE_COUNT; ++techId) {
    std::ostringstream sectionName;
    sectionName << "Tech #" << techId;
    iniProc.setSection(sectionName.str(), getTechName(techId));

    iniProc.process(data.mineralCost[techId],   "Mineral Cost");
    iniProc.process(data.gasCost[techId],       "Gas Cost");
    iniProc.process(data.researchTime[techId],  "Research Time", makeTimeComment);
    iniProc.process(data.energyCost[techId],    "Energy Cost");

    iniProc.process(data.unknown[techId],       "Unknown");
    
    iniProc.process(data.icon[techId],          "Icon");
    iniProc.process(data.label[techId],         "Label", makeStatTxtTblComment);
    
    iniProc.process(data.race[techId],          "Race");
    iniProc.process(data.unused[techId],        "Unused");
    iniProc.process(data.isBroodWarOnly[techId],"Is Brood War Only");
  }
}

} //datcc
