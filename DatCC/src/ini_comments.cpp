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

std::string makeUnitComment(const std::string &iniKey, int unitId) {
  std::ostringstream os;
  os << iniKey << " = " << unitId;
  os.width(32 - os.tellp());
  os << "; " << getUnitName(unitId);
  return os.str().erase(0, iniKey.size() + 3);
}

std::string makeWeaponComment(const std::string &iniKey, int weaponId) {
  std::ostringstream os;
  os << iniKey << " = " << weaponId;
  os.width(32 - os.tellp());
  os << "; " << getWeaponName(weaponId);
  return os.str().erase(0, iniKey.size() + 3);  
}

std::string makeFlingyComment(const std::string &iniKey, int flingyId) {
  std::ostringstream os;
  os << iniKey << " = " << flingyId;
  os.width(32 - os.tellp());
  os << "; " << getFlingyName(flingyId);
  return os.str().erase(0, iniKey.size() + 3);
}

std::string makeSpriteComment(const std::string &iniKey, int spriteId) {
  std::ostringstream os;
  os << iniKey << " = " << spriteId;
  os.width(32 - os.tellp());
  os << "; " << getSpriteName(spriteId);
  return os.str().erase(0, iniKey.size() + 3);
}

std::string makeImageComment(const std::string &iniKey, int imageId) {
  std::ostringstream os;
  os << iniKey << " = " << imageId;
  os.width(32 - os.tellp());
  os << "; " << getImageName(imageId);
  return os.str().erase(0, iniKey.size() + 3);  
}

std::string makeOrderComment(const std::string &iniKey, int orderId) {
  std::ostringstream os;
  os << iniKey << " = " << orderId;
  os.width(32 - os.tellp());
  os << "; " << getOrderName(orderId);
  return os.str().erase(0, iniKey.size() + 3);  
}

std::string makeTblComment(const std::string &iniKey, int stringIndex) {
  std::ostringstream os;
  os << iniKey << " = " << stringIndex;
  os.width(32 - os.tellp());
  os << "; " << statTxtTbl.getEscapedString(stringIndex);
  return os.str().erase(0, iniKey.size() + 3);  
}


} //datcc
