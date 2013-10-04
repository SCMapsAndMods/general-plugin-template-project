#include "data.h"
#include <fstream>
#include <iostream>

namespace datcc {

//-------- Default DAT paths --------//

char DefaultDat<UnitsDat>::path[]     = "data/units.dat";
char DefaultDat<WeaponsDat>::path[]   = "data/weapons.dat";
char DefaultDat<FlingyDat>::path[]    = "data/flingy.dat";
char DefaultDat<SpritesDat>::path[]   = "data/sprites.dat";
char DefaultDat<ImagesDat>::path[]    = "data/images.dat";
char DefaultDat<UpgradesDat>::path[]  = "data/upgrades.dat";

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

//@TODO Add all DAT types so that these constants can find a home
const int ORDER_TYPE_COUNT = 189;

std::string unitNames[UNIT_TYPE_COUNT + 1];
std::string weaponNames[WEAPON_TYPE_COUNT + 1];
std::string flingyNames[FLINGY_TYPE_COUNT];
std::string spriteNames[SPRITE_TYPE_COUNT];
std::string imageNames[IMAGE_TYPE_COUNT];
std::string upgradeNames[UPGRADE_TYPE_COUNT + 1];
std::string orderNames[ORDER_TYPE_COUNT + 1];

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

void loadData() {
  loadNameFile("data/units.txt",    unitNames,    sizeof(unitNames));
  loadNameFile("data/weapons.txt",  weaponNames,  sizeof(weaponNames));
  loadNameFile("data/flingy.txt",   flingyNames,  sizeof(flingyNames));
  loadNameFile("data/sprites.txt",  spriteNames,  sizeof(spriteNames));
  loadNameFile("data/images.txt",   imageNames,   sizeof(imageNames));
  loadNameFile("data/upgrades.txt", upgradeNames, sizeof(upgradeNames));
  loadNameFile("data/orders.txt",   orderNames,   sizeof(orderNames));

  if (statTxtTbl.loadFile((getCurrentProgramDir() + "data/stat_txt.tbl").c_str())) {
    std::cerr << "Error: Cannot read default stat_txt.tbl" << std::endl;
    exit(1);
  }
  
  if (imagesTbl.loadFile((getCurrentProgramDir() + "data/images.tbl").c_str())) {
    std::cerr << "Error: Cannot read default images.tbl" << std::endl;
    exit(1);
  }
}

//-------- DAT entry names --------//

static const std::string invalidIndexMsg("invalid index");

const std::string& getUnitName(int unitId) {
  if (0 <= unitId && unitId < sizeof(unitNames))
    return unitNames[unitId];
  return invalidIndexMsg;
}

const std::string& getWeaponName(int weaponId) {
  if (0 <= weaponId && weaponId < sizeof(weaponNames))
    return weaponNames[weaponId];
  return invalidIndexMsg;
}

const std::string& getFlingyName(int flingyId) {
  if (0 <= flingyId && flingyId < sizeof(flingyNames))
    return flingyNames[flingyId];
  return invalidIndexMsg;
}

const std::string& getSpriteName(int spriteId) {
  if (0 <= spriteId && spriteId < sizeof(spriteNames))
    return spriteNames[spriteId];
  return invalidIndexMsg;
}

const std::string& getImageName(int imageId) {
  if (0 <= imageId && imageId < sizeof(imageNames))
    return imageNames[imageId];
  return invalidIndexMsg;
}

const std::string& getUpgradeName(int upgradeId) {
  if (0 <= upgradeId && upgradeId < sizeof(upgradeNames))
    return upgradeNames[upgradeId];
  return invalidIndexMsg;
}

const std::string& getOrderName(int orderId) {
  if (0 <= orderId && orderId < sizeof(orderNames))
    return orderNames[orderId];
  return invalidIndexMsg;
}

} //datcc
