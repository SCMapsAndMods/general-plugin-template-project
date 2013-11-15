#include "ini_comments.h"
#include "data.h"
#include "flags.h"
#include <sstream>
#include <vector>

namespace datcc {

//-------- INI key/value comments --------//

std::string makeCommentPadding(int id, size_t keyStrSize) {
  std::ostringstream os;
  os << id;
  os.width(31 - os.tellp() - keyStrSize); //Align to 32 characters
  os << "; ";
  return os.str();
}

std::string makeCommentPadding(const char *flagStr, size_t keyStrSize) {
  std::ostringstream os;
  os << flagStr;
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

std::string makeSfxdataTblComment(int stringIndex, size_t keyStrSize) {
  return makeCommentPadding(stringIndex, keyStrSize)
    + "sfxdata.tbl: " + sfxdataTbl.getEscapedString(stringIndex);
}


std::string makeTimeComment(int time, size_t keyStrSize) {
  std::ostringstream os;
  os.setf(std::ios::fixed, std::ios::floatfield);
  os.precision(2);
  os << time / 15. << " sec on Normal, " << time / 24. << " sec on Fastest";
  return makeCommentPadding(time, keyStrSize) + os.str();
}

std::string makeHpAmountComment (int hp, size_t keyStrSize) {
  std::ostringstream os;
  os << hp / 256. << " HP";
  return makeCommentPadding(hp, keyStrSize) + os.str();
}

std::string makeSpeedComment(int speed, size_t keyStrSize) {
  std::ostringstream os;
  os << speed / 256. << " pixels per frame";
  return makeCommentPadding(speed, keyStrSize) + os.str();
}

std::string makeSupplyComment(int supply, size_t keyStrSize) {
  std::ostringstream os;
  os << supply / 2. << " supply in game";
  return makeCommentPadding(supply, keyStrSize) + os.str();
}

std::string makeWeaponRangeComment(int weaponRange, size_t keyStrSize) {
  std::ostringstream os;
  os << weaponRange / 32. << " matrix distance in game";
  return makeCommentPadding(weaponRange, keyStrSize) + os.str();
}

std::string makeAngleComment(int brad, size_t keyStrSize) {
  std::ostringstream os;
  os << brad * 1.40625 << " degrees";
  return makeCommentPadding(brad, keyStrSize) + os.str();
}

extern std::vector<std::string> imagesDatDrawingFunctions;
std::string makeDrawingFunctionComment(int id, size_t keyStrSize) {
  if (0 <= id && id < imagesDatDrawingFunctions.size())
    return makeCommentPadding(id, keyStrSize) + imagesDatDrawingFunctions[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";  
}

extern std::vector<std::string> imagesDatRemappings;
std::string makeRemappingComment(int id, size_t keyStrSize) {
  if (0 <= id && id < imagesDatRemappings.size())
    return makeCommentPadding(id, keyStrSize) + imagesDatRemappings[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";
}

const char damageTypes[][20] = {
  "Independent", "Explosive", "Concussive", "Normal", "Ignore Armor"
};
std::string makeDamageTypeComment(int id, size_t keyStrSize) {
  if (0 <= id && id < ARRAY_LEN(damageTypes))
    return makeCommentPadding(id, keyStrSize) + damageTypes[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";
}

const char weaponFlingyActions[][30] = {
  "Fly & don't follow",
  "Fly & follow",
  "Appear on target unit",
  "Persist on target site",
  "Appear on target site",
  "Appear on attacker",
  "Attack & Self-Destruct",
  "Bounce",
  "Attack 3x3 Area",
  "Go to max range"
};
std::string makeWeaponFlingyActionComment(int id, size_t keyStrSize) {
  if (0 <= id && id < ARRAY_LEN(weaponFlingyActions))
    return makeCommentPadding(id, keyStrSize) + weaponFlingyActions[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";
}

const char weaponEffects[][20] = {
  "None",
  "Normal Hit",
  "Splash (Radial)",
  "Splash (Enemy)",
  "Lockdown",
  "Nuclear Missile",
  "Parasite",
  "Broodlings",
  "EMP Shockwave",
  "Irradiate",
  "Ensnare",
  "Plague",
  "Stasis Field",
  "Dark Swarm",
  "Consume",
  "Yamato Gun",
  "Restoration",
  "Disruption Web",
  "Corrosive Acid",
  "Mind Control",
  "Feedback",
  "Optical Flare",
  "Maelstrom",
  "Unknown (Crash)",
  "Splash (Air)",
};
std::string makeWeaponEffectComment(int id, size_t keyStrSize) {
  if (0 <= id && id < ARRAY_LEN(weaponEffects))
    return makeCommentPadding(id, keyStrSize) + weaponEffects[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";
}

const char flingyDatControlTypes[][50] = {
  "Accelerate / decelerate", "Accelerate / instant stop", "Iscript-controlled"
};
std::string makeFlingyControlTypeComment(int id, size_t keyStrSize) {
  if (0 <= id && id < ARRAY_LEN(flingyDatControlTypes))
    return makeCommentPadding(id, keyStrSize) + flingyDatControlTypes[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";
}

const char iscriptAnimations[][20] = {
  "Init",
  "Death",
  "GndAttkInit",
  "AirAttkInit",
  "Unused1",
  "GndAttkRpt",
  "AirAttkRpt",
  "CastSpell",
  "GndAttkToIdle",
  "AirAttkToIdle",
  "Unused2",
  "Walking",
  "WalkingToIdle",
  "SpecialState1",
  "SpecialState2",
  "AlmostBuilt",
  "Built",
  "Landing",
  "LiftOff",
  "IsWorking",
  "WorkingToIdle",
  "WarpIn",
  "Unused3",
  "StarEditInit",
  "Disable",
  "Burrow",
  "Unburrow",
  "Enable",
  "None"
};
std::string makeIscriptAnimComment(int id, size_t keyStrSize) {
  if (0 <= id && id < ARRAY_LEN(iscriptAnimations))
    return makeCommentPadding(id, keyStrSize) + iscriptAnimations[id];
  else
    return makeCommentPadding(id, keyStrSize) + "Invalid value";
}

} //datcc
