#pragma once
#include <string>

namespace datcc {

std::string makeUnitSectionComment(int unitId);
std::string makeWeaponSectionComment(int weaponId);
std::string makeFlingySectionComment(int flingyId);

std::string makeUnitComment(const std::string &iniKey, int unitId);
std::string makeWeaponComment(const std::string &iniKey, int weaponId);
std::string makeFlingyComment(const std::string &iniKey, int unitId);
std::string makeSpriteComment(const std::string &iniKey, int spriteId);
std::string makeImageComment(const std::string &iniKey, int imageId);
std::string makeOrderComment(const std::string &iniKey, int orderId);
std::string makeTblComment(const std::string &iniKey, int stringIndex);

} //datcc
