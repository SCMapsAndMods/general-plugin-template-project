#pragma once
#include <string>

namespace datcc {

void compileUnits   (const std::string &inputPath, const std::string &basePath);
void compileWeapons (const std::string &inputPath, const std::string &basePath);
void compileFlingy  (const std::string &inputPath, const std::string &basePath);
void compileSprites (const std::string &inputPath, const std::string &basePath);
void compileImages  (const std::string &inputPath, const std::string &basePath);
void compileUpgrades(const std::string &inputPath, const std::string &basePath);
void compileTech    (const std::string &inputPath, const std::string &basePath);
void compileOrders  (const std::string &inputPath, const std::string &basePath);

void decompileUnits   (const std::string &inputPath);
void decompileWeapons (const std::string &inputPath);
void decompileFlingy  (const std::string &inputPath);
void decompileSprites (const std::string &inputPath);
void decompileImages  (const std::string &inputPath);
void decompileUpgrades(const std::string &inputPath);
void decompileTech    (const std::string &inputPath);
void decompileOrders  (const std::string &inputPath);

} //datcc
