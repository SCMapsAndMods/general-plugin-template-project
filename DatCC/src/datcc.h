#pragma once
#include <string>

namespace datcc {

void compileUnits   (const std::string &inputPath);
void compileWeapons (const std::string &inputPath);
void compileFlingy  (const std::string &inputPath);
void compileSprites (const std::string &inputPath);
void compileImages  (const std::string &inputPath);
void compileUpgrades(const std::string &inputPath);
void compileTech    (const std::string &inputPath);
void compileOrders  (const std::string &inputPath);

void decompileUnits   (const std::string &inputPath);
void decompileWeapons (const std::string &inputPath);
void decompileFlingy  (const std::string &inputPath);
void decompileSprites (const std::string &inputPath);
void decompileImages  (const std::string &inputPath);
void decompileUpgrades(const std::string &inputPath);
void decompileTech    (const std::string &inputPath);
void decompileOrders  (const std::string &inputPath);

} //datcc
