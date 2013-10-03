#include "ini_comments.h"
#include "data.h"
#include <sstream>

namespace datcc {

//-------- INI section comments --------//

std::string makeUnitSectionComment(int unitId) {
  return "; " + getUnitName(unitId);  
}

std::string makeWeaponSectionComment(int weaponId) {
  return "; " + getWeaponName(weaponId);  
}

std::string makeFlingySectionComment(int flingyId) {
  return "; " + getFlingyName(flingyId);  
}

//-------- INI key/value comments --------//

std::string makeUnitComment(int unitId, size_t keyStrSize) {
  std::ostringstream os;
  os << unitId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getUnitName(unitId);
  return os.str();
}

std::string makeWeaponComment(int weaponId, size_t keyStrSize) {
  std::ostringstream os;
  os << weaponId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getWeaponName(weaponId);
  return os.str();
}

std::string makeFlingyComment(int flingyId, size_t keyStrSize) {
  std::ostringstream os;
  os << flingyId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getFlingyName(flingyId);
  return os.str();
}

std::string makeSpriteComment(int spriteId, size_t keyStrSize) {
  std::ostringstream os;
  os << spriteId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getSpriteName(spriteId);
  return os.str();
}

std::string makeImageComment(int imageId, size_t keyStrSize) {
  std::ostringstream os;
  os << imageId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getImageName(imageId);
  return os.str();
}

std::string makeOrderComment(int orderId, size_t keyStrSize) {
  std::ostringstream os;
  os << orderId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getOrderName(orderId);
  return os.str();
}

std::string makeUpgradeComment(int upgradeId, size_t keyStrSize) {
  std::ostringstream os;
  os << upgradeId;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << getUpgradeName(upgradeId);
  return os.str();
}

std::string makeTblComment(int stringIndex, size_t keyStrSize) {
  std::ostringstream os;
  os << stringIndex;
  os.width(32 - os.tellp() - keyStrSize);
  os << "; " << statTxtTbl.getEscapedString(stringIndex);
  return os.str();
}


} //datcc
