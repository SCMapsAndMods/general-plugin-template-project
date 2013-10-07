#pragma once
#include "DatInterface.h"
#include <sstream>

namespace datcc {

const int WEAPON_TYPE_COUNT = 130;

#pragma pack(1)

struct WeaponsDatFile {
  WORD  label               [WEAPON_TYPE_COUNT];
  DWORD flingy              [WEAPON_TYPE_COUNT];
  BYTE  techHint            [WEAPON_TYPE_COUNT];
  WORD  targetFlags         [WEAPON_TYPE_COUNT];
  DWORD minRange            [WEAPON_TYPE_COUNT];
  DWORD maxRange            [WEAPON_TYPE_COUNT];
  BYTE  upgrade             [WEAPON_TYPE_COUNT];
  BYTE  damageType          [WEAPON_TYPE_COUNT];
  BYTE  flingyAction        [WEAPON_TYPE_COUNT];
  BYTE  removeTimer         [WEAPON_TYPE_COUNT];
  BYTE  effect              [WEAPON_TYPE_COUNT];
  WORD  innerSplashRadius   [WEAPON_TYPE_COUNT];
  WORD  mediumSplashRadius  [WEAPON_TYPE_COUNT];
  WORD  outerSplashRadius   [WEAPON_TYPE_COUNT];
  WORD  damage              [WEAPON_TYPE_COUNT];
  WORD  damageBonus         [WEAPON_TYPE_COUNT];
  BYTE  cooldown            [WEAPON_TYPE_COUNT];
  BYTE  damageFactor        [WEAPON_TYPE_COUNT];
  BYTE  attackAngle         [WEAPON_TYPE_COUNT];
  BYTE  launchSpin          [WEAPON_TYPE_COUNT];
  SBYTE forwardOffset       [WEAPON_TYPE_COUNT];
  SBYTE verticalOffset      [WEAPON_TYPE_COUNT];
  WORD  targetErrorMsg      [WEAPON_TYPE_COUNT];
  WORD  icon                [WEAPON_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(WeaponsDatFile) == 5460);

class WeaponsDat: public DatInterface {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);

  private:
    WeaponsDatFile data;
};

template <class IniProcT>
void WeaponsDat::processIni(IniProcT &iniProc) {
  for (int weaponId = 0; weaponId < WEAPON_TYPE_COUNT; ++weaponId) {
    std::ostringstream sectionName;
    sectionName << "Weapon #" << weaponId;
    iniProc.setSection(sectionName.str(), getWeaponName(weaponId));
    
    iniProc.process(data.label[weaponId],           "Label", makeStatTxtTblComment);

    iniProc.process(data.techHint[weaponId],        "Tech Hint", makeTechComment);

    iniProc.processFlags(data.targetFlags[weaponId], "Target Flags");
    iniProc.process(data.targetErrorMsg[weaponId],  "Target Error Message", makeStatTxtTblComment);

    iniProc.process(data.minRange[weaponId],        "Minimum Range");
    iniProc.process(data.maxRange[weaponId],        "Maximum Range");

    iniProc.process(data.damage[weaponId],          "Damage");
    iniProc.process(data.damageBonus[weaponId],     "Damage Bonus");
    iniProc.process(data.upgrade[weaponId],         "Upgrade", makeUpgradeComment);
    iniProc.process(data.damageType[weaponId],      "Damage Type");
    iniProc.process(data.cooldown[weaponId],        "Cooldown");
    iniProc.process(data.damageFactor[weaponId],    "Damage Factor");

    iniProc.process(data.effect[weaponId],          "Effect");
    iniProc.process(data.innerSplashRadius[weaponId], "Inner Splash Radius");
    iniProc.process(data.mediumSplashRadius[weaponId], "Medium Splash Radius");
    iniProc.process(data.outerSplashRadius[weaponId], "Outer Splash Radius");

    iniProc.process(data.flingy[weaponId],          "Flingy", makeFlingyComment);
    iniProc.process(data.flingyAction[weaponId],    "Flingy Action");
    iniProc.process(data.removeTimer[weaponId],     "Remove Timer", makeTimeComment);
    iniProc.process(data.attackAngle[weaponId],     "Attack Angle");
    iniProc.process(data.launchSpin[weaponId],      "Launch Spin");
    iniProc.process(data.forwardOffset[weaponId],   "Forward Offset");
    iniProc.process(data.verticalOffset[weaponId],  "Vertical Offset");

    iniProc.process(data.icon[weaponId],            "Icon", makeIconNameComment);
  }
}

} //datcc
