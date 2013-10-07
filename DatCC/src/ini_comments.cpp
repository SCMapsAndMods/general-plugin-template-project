#include "ini_comments.h"
#include "data.h"
#include <sstream>

namespace datcc {

//-------- INI key/value comments --------//

std::string makeCommentPadding(int id, size_t keyStrSize) {
  std::ostringstream os;
  os << id;
  os.width(31 - os.tellp() - keyStrSize); //Align to 32 characters
  os << "; ";
  return os.str();
}

std::string makeUnitComment(int unitId, size_t keyStrSize) {
  return makeCommentPadding(unitId, keyStrSize) + getUnitName(unitId);
}

std::string makeWeaponComment(int weaponId, size_t keyStrSize) {
  return makeCommentPadding(weaponId, keyStrSize) + getWeaponName(weaponId);
}

std::string makeFlingyComment(int flingyId, size_t keyStrSize) {
  return makeCommentPadding(flingyId, keyStrSize) + getFlingyName(flingyId);
}

std::string makeSpriteComment(int spriteId, size_t keyStrSize) {
  return makeCommentPadding(spriteId, keyStrSize) + getSpriteName(spriteId);
}

std::string makeImageComment(int imageId, size_t keyStrSize) {
  return makeCommentPadding(imageId, keyStrSize) + getImageName(imageId);
}

std::string makeOrderComment(int orderId, size_t keyStrSize) {
  return makeCommentPadding(orderId, keyStrSize) + getOrderName(orderId);
}

std::string makeUpgradeComment(int upgradeId, size_t keyStrSize) {
  return makeCommentPadding(upgradeId, keyStrSize) + getUpgradeName(upgradeId);
}

std::string makeTechComment(int techId, size_t keyStrSize) {
  return makeCommentPadding(techId, keyStrSize) + getTechName(techId);
}

std::string makeIconNameComment(int iconId, size_t keyStrSize) {
  return makeCommentPadding(iconId, keyStrSize) + getIconName(iconId);
}


std::string makeStatTxtTblComment(int stringIndex, size_t keyStrSize) {
  return makeCommentPadding(stringIndex, keyStrSize)
    + "stat_txt.tbl: " + statTxtTbl.getEscapedString(stringIndex);
}

std::string makeImagesTblComment(int stringIndex, size_t keyStrSize) {
  return makeCommentPadding(stringIndex, keyStrSize)
    + "images.tbl: " + imagesTbl.getEscapedString(stringIndex);
}


std::string makeTimeComment(int time, size_t keyStrSize) {
  std::ostringstream os;
  os.setf(std::ios::fixed, std::ios::floatfield);
  os.precision(2);
  os << time / 15. << " sec on Normal, " << time / 24. << " sec on Fastest";
  return makeCommentPadding(time, keyStrSize) + os.str();
}

} //datcc
