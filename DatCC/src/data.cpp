#include "data.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace datcc {

//-------- Default DAT paths --------//

char DefaultDat<UnitsDat>::path[]     = "defaults/units.dat";
char DefaultDat<WeaponsDat>::path[]   = "defaults/weapons.dat";
char DefaultDat<FlingyDat>::path[]    = "defaults/flingy.dat";
char DefaultDat<SpritesDat>::path[]   = "defaults/sprites.dat";
char DefaultDat<ImagesDat>::path[]    = "defaults/images.dat";
char DefaultDat<UpgradesDat>::path[]  = "defaults/upgrades.dat";
char DefaultDat<TechdataDat>::path[]  = "defaults/techdata.dat";
char DefaultDat<SfxdataDat>::path[]   = "defaults/sfxdata.dat";
char DefaultDat<OrdersDat>::path[]    = "defaults/orders.dat";

//-------- Current program directory --------//

static std::string currentProgramDir;

void setCurrentProgramDir(const char *argv_0) {
  currentProgramDir = argv_0;
  size_t pos = currentProgramDir.find_last_of("\\/");
  if (pos == std::string::npos) //No directory separator--use relative path
    currentProgramDir = "./";
  else
    currentProgramDir.erase(pos + 1);
}

const std::string& getCurrentProgramDir() {
  return currentProgramDir;
}

//-------- Text and TBL loader --------//

std::string unitNames[UNIT_TYPE_COUNT + 1];
std::string weaponNames[WEAPON_TYPE_COUNT + 1];
std::string flingyNames[FLINGY_TYPE_COUNT];
std::string spriteNames[SPRITE_TYPE_COUNT];
std::string imageNames[IMAGE_TYPE_COUNT];
std::string upgradeNames[UPGRADE_TYPE_COUNT + 1];
std::string techNames[TECH_TYPE_COUNT + 1];
std::string orderNames[ORDER_TYPE_COUNT + 1];

std::vector<std::string> iconNames;

std::vector<std::string> imagesDatDrawingFunctions;
std::vector<std::string> imagesDatRemappings;

void loadNameFile(const char *basePath, std::string arr[], size_t arr_size) {
  std::string filePath = getCurrentProgramDir() + basePath;

  std::ifstream in(filePath.c_str());
  if (in.fail()) {
    std::cerr << "Critical error: Cannot open settings file " << filePath << std::endl;
    exit(1);
  }

  for (unsigned int i = 0; i < arr_size && !in.eof(); ++i)
    std::getline(in, arr[i]);
}

void loadNameFile(const char *basePath, std::vector<std::string> &names) {
  std::string filePath = getCurrentProgramDir() + basePath;

  std::ifstream in(filePath.c_str());
  if (in.fail()) {
    std::cerr << "Critical error: Cannot open settings file " << filePath << std::endl;
    exit(1);
  }

  std::string temp;
  while (!in.eof()) {
    std::getline(in, temp);
    names.push_back(temp);
  }
}

void loadData() {
  loadNameFile("data/units.txt",    unitNames,    ARRAY_LEN(unitNames));
  loadNameFile("data/weapons.txt",  weaponNames,  ARRAY_LEN(weaponNames));
  loadNameFile("data/flingy.txt",   flingyNames,  ARRAY_LEN(flingyNames));
  loadNameFile("data/sprites.txt",  spriteNames,  ARRAY_LEN(spriteNames));
  loadNameFile("data/images.txt",   imageNames,   ARRAY_LEN(imageNames));
  loadNameFile("data/upgrades.txt", upgradeNames, ARRAY_LEN(upgradeNames));
  loadNameFile("data/techdata.txt", techNames,    ARRAY_LEN(techNames));
  loadNameFile("data/orders.txt",   orderNames,   ARRAY_LEN(orderNames));
  
  loadNameFile("data/icons.txt",    iconNames);

  loadNameFile("data/DrawingFunctions.txt", imagesDatDrawingFunctions);
  loadNameFile("data/Remappings.txt",       imagesDatRemappings);

  if (statTxtTbl.loadFile(getCurrentProgramDir() + "defaults/stat_txt.tbl")) {
    std::cerr << "Error: Cannot read default stat_txt.tbl" << std::endl;
    exit(1);
  }

  if (imagesTbl.loadFile(getCurrentProgramDir() + "defaults/images.tbl")) {
    std::cerr << "Error: Cannot read default images.tbl" << std::endl;
    exit(1);
  }
  
  if (sfxdataTbl.loadFile(getCurrentProgramDir() + "defaults/sfxdata.tbl")) {
    std::cerr << "Error: Cannot read default sfxdata.tbl" << std::endl;
    exit(1);
  }
}

//-------- DAT entry names --------//

static const std::string invalidIndexMsg("invalid index");

const std::string& getUnitName(int unitId) {
  if (0 <= unitId && unitId < ARRAY_LEN(unitNames))
    return unitNames[unitId];
  return invalidIndexMsg;
}

const std::string& getWeaponName(int weaponId) {
  if (0 <= weaponId && weaponId < ARRAY_LEN(weaponNames))
    return weaponNames[weaponId];
  return invalidIndexMsg;
}

const std::string& getFlingyName(int flingyId) {
  if (0 <= flingyId && flingyId < ARRAY_LEN(flingyNames))
    return flingyNames[flingyId];
  return invalidIndexMsg;
}

const std::string& getSpriteName(int spriteId) {
  if (0 <= spriteId && spriteId < ARRAY_LEN(spriteNames))
    return spriteNames[spriteId];
  return invalidIndexMsg;
}

const std::string& getImageName(int imageId) {
  if (0 <= imageId && imageId < ARRAY_LEN(imageNames))
    return imageNames[imageId];
  return invalidIndexMsg;
}

const std::string& getUpgradeName(int upgradeId) {
  if (0 <= upgradeId && upgradeId < ARRAY_LEN(upgradeNames))
    return upgradeNames[upgradeId];
  return invalidIndexMsg;
}

const std::string& getTechName(int techId) {
  if (0 <= techId && techId < ARRAY_LEN(techNames))
    return techNames[techId];
  return invalidIndexMsg;
}

const std::string& getOrderName(int orderId) {
  if (0 <= orderId && orderId < ARRAY_LEN(orderNames))
    return orderNames[orderId];
  return invalidIndexMsg;
}

const std::string& getIconName(int iconId) {
  static const std::string noIconMsg("No icon");
  if (0 <= iconId && iconId < iconNames.size())
    return iconNames.at(iconId);
  else if (iconId == -1)
    return noIconMsg;
  return invalidIndexMsg;
}

} //datcc
