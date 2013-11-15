#include "DatInterface.h"

namespace datcc {

//Based on http://modcrafters.com/wiki/index.php?title=Orders.dat

const int ORDER_TYPE_COUNT = 189;

#pragma pack(1)

struct OrdersDatFile {
  SWORD label               [ORDER_TYPE_COUNT];
  BYTE  useWeaponTargeting  [ORDER_TYPE_COUNT];
  BYTE  unused2             [ORDER_TYPE_COUNT];
  BYTE  unused3             [ORDER_TYPE_COUNT];
  BYTE  unknown4            [ORDER_TYPE_COUNT];
  BYTE  unused5             [ORDER_TYPE_COUNT];
  BYTE  canBeInterrupted    [ORDER_TYPE_COUNT];
  BYTE  unknown7            [ORDER_TYPE_COUNT];
  BYTE  canBeQueued         [ORDER_TYPE_COUNT];
  BYTE  unknown9            [ORDER_TYPE_COUNT];
  BYTE  canBeObstructed     [ORDER_TYPE_COUNT];
  BYTE  unknown11           [ORDER_TYPE_COUNT];
  BYTE  unused12            [ORDER_TYPE_COUNT];
  BYTE  targetingWeapon     [ORDER_TYPE_COUNT];
  BYTE  energyCostTech      [ORDER_TYPE_COUNT];
  BYTE  iscriptAnimation    [ORDER_TYPE_COUNT];
  SWORD hilightedIcon       [ORDER_TYPE_COUNT];
  SWORD unknown17           [ORDER_TYPE_COUNT];
  BYTE  obscuredOrder       [ORDER_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(OrdersDatFile) == 4158);

class OrdersDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    OrdersDatFile data;
};

template <class IniProcT>
void OrdersDat::processIni(IniProcT &iniProc) {
  for (int orderId = 0; orderId < ORDER_TYPE_COUNT; ++orderId) {
    std::ostringstream sectionName;
    sectionName << "Order #" << orderId;
    iniProc.setSection(sectionName.str(), getOrderName(orderId));
    
    iniProc.process(data.label[orderId],            "Label", makeStatTxtTblComment);
    iniProc.process(data.useWeaponTargeting[orderId], "Use Weapon Targeting");
    iniProc.process(data.unused2[orderId],          "Unused2");
    iniProc.process(data.unused3[orderId],          "Unused3");
    iniProc.process(data.unknown4[orderId],         "Unknown4");
    iniProc.process(data.unused5[orderId],          "Unused5");
    iniProc.process(data.canBeInterrupted[orderId], "Can Be Interrupted");
    iniProc.process(data.unknown7[orderId],         "Unknown7");
    iniProc.process(data.canBeQueued[orderId],      "Can Be Queued");
    iniProc.process(data.unknown9[orderId],         "Unknown9");
    iniProc.process(data.canBeObstructed[orderId],  "Can Be Obstructed");
    iniProc.process(data.unknown11[orderId],        "Unknown11");
    iniProc.process(data.unused12[orderId],         "Unused12");
    iniProc.process(data.targetingWeapon[orderId],  "Targeting Weapon", makeWeaponComment);
    iniProc.process(data.energyCostTech[orderId],   "Energy Cost Tech", makeTechComment);
    iniProc.process(data.iscriptAnimation[orderId], "Iscript Animation", makeIscriptAnimComment);
    iniProc.process(data.hilightedIcon[orderId],    "Highlighted Icon", makeIconNameComment);
    iniProc.process(data.unknown17[orderId],        "Unknown17");
    iniProc.process(data.obscuredOrder[orderId],    "Obscured Order", makeOrderComment);
  }
}

} //datcc
