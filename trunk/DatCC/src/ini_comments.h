#pragma once
#include <string>

namespace datcc {

//Value describes data from text files
std::string makeUnitComment   (int unitId,    size_t keyStrSize);
std::string makeWeaponComment (int weaponId,  size_t keyStrSize);
std::string makeFlingyComment (int unitId,    size_t keyStrSize);
std::string makeSpriteComment (int spriteId,  size_t keyStrSize);
std::string makeImageComment  (int imageId,   size_t keyStrSize);
std::string makeUpgradeComment(int upgradeId, size_t keyStrSize);
std::string makeTechComment   (int techId,    size_t keyStrSize);
std::string makeOrderComment  (int orderId,   size_t keyStrSize);
std::string makeIconNameComment(int iconId,   size_t keyStrSize);

//Value is an index of a TBL file
std::string makeStatTxtTblComment (int stringIndex, size_t keyStrSize);
std::string makeImagesTblComment  (int stringIndex, size_t keyStrSize);
std::string makeSfxdataTblComment (int stringIndex, size_t keyStrSize);

//Value has other meanings
std::string makeTimeComment     (int time,    size_t keyStrSize);
std::string makeHpAmountComment (int hp,      size_t keyStrSize);
std::string makeShieldAmtComment(int shields, size_t keyStrSize);
std::string makeSpeedComment    (int speed,   size_t keyStrSize);

} //datcc
