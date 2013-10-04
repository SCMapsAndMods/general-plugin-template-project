#pragma once
#include "DatInterface.h"
#include <sstream>

namespace datcc {

//Based on http://modcrafters.com/wiki/index.php?title=Upgrades.dat

const int UPGRADE_TYPE_COUNT = 61;

#pragma pack(1)

struct UpgradesDatFile {
  WORD  mineralCostBase     [UPGRADE_TYPE_COUNT];
  WORD  mineralCostFactor   [UPGRADE_TYPE_COUNT];
  WORD  gasCostBase         [UPGRADE_TYPE_COUNT];
  WORD  gasCostFactor       [UPGRADE_TYPE_COUNT];
  WORD  researchTimeBase    [UPGRADE_TYPE_COUNT];
  WORD  researchTimeFactor  [UPGRADE_TYPE_COUNT];
  WORD  unknown             [UPGRADE_TYPE_COUNT];
  WORD  icon                [UPGRADE_TYPE_COUNT];
  SWORD label               [UPGRADE_TYPE_COUNT];
  BYTE  race                [UPGRADE_TYPE_COUNT];
  BYTE  maxRepeats          [UPGRADE_TYPE_COUNT];
  BYTE  isBroodWarOnly      [UPGRADE_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(UpgradesDatFile) == 1281);


class UpgradesDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    UpgradesDatFile data;
};

template <class IniProcT>
void UpgradesDat::processIni(IniProcT &iniProc) {
  for (int upgradeId = 0; upgradeId < UPGRADE_TYPE_COUNT; ++upgradeId) {
    std::ostringstream sectionName;
    sectionName << "Upgrade #" << upgradeId;
    iniProc.setSection(sectionName.str(), getUpgradeName(upgradeId));

    iniProc.process(data.mineralCostBase[upgradeId],    "Mineral Cost Base");
    iniProc.process(data.mineralCostFactor[upgradeId],  "Mineral Cost Factor");
    iniProc.process(data.gasCostBase[upgradeId],        "Gas Cost Base");
    iniProc.process(data.gasCostFactor[upgradeId],      "Gas Cost Factor");
    iniProc.process(data.researchTimeBase[upgradeId],   "Research Time Base", makeTimeComment);
    iniProc.process(data.researchTimeFactor[upgradeId], "Research Time Factor", makeTimeComment);

    iniProc.process(data.unknown[upgradeId],            "Unknown");
    
    iniProc.process(data.icon[upgradeId],               "Icon");
    iniProc.process(data.label[upgradeId],              "Label", makeStatTxtTblComment);
    
    iniProc.process(data.race[upgradeId],               "Race");
    iniProc.process(data.maxRepeats[upgradeId],         "Max Repeats");
    iniProc.process(data.isBroodWarOnly[upgradeId],     "Is Brood War Only");
  }
}

} //datcc
