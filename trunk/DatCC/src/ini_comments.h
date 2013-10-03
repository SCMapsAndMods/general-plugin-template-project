#pragma once
#include <string>

namespace datcc {

std::string makeUnitSectionComment(int unitId);
std::string makeWeaponSectionComment(int weaponId);
std::string makeFlingySectionComment(int flingyId);

std::string makeUnitComment(int unitId, size_t keyStrSize);
std::string makeWeaponComment(int weaponId, size_t keyStrSize);
std::string makeFlingyComment(int unitId, size_t keyStrSize);
std::string makeSpriteComment(int spriteId, size_t keyStrSize);
std::string makeImageComment(int imageId, size_t keyStrSize);
std::string makeOrderComment(int orderId, size_t keyStrSize);
std::string makeTblComment(int stringIndex, size_t keyStrSize);

} //datcc
