#pragma once
#include "formats/UnitsDat.h"
#include "formats/WeaponsDat.h"
#include "formats/FlingyDat.h"
#include "formats/TblFile.h"
#include <string>

namespace datcc {

//Load text files and TBL files
void loadData();

/// Set the current directory of the program for loading default DATs and TXTs.
void setCurrentProgramDir(const char *argv_0);

/// Load the current directory of the program saved by setCurrentProgramDir().
const std::string& getCurrentProgramDir();

/// Retrieve path of default DAT files
template <class DatT> class DefaultDat { public: static char path[]; };

/// Retrieve DAT entry names loaded from text files.
const std::string& getUnitName(int unitId);
const std::string& getWeaponName(int weaponId);
const std::string& getFlingyName(int flingyId);
const std::string& getSpriteName(int spriteId);
const std::string& getImageName(int imageId);
const std::string& getUpgradeName(int upgradeId);
const std::string& getOrderName(int orderId);

} //datcc
