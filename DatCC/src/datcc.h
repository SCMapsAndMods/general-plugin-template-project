#pragma once
#include <string>

namespace datcc {

void compileUnits   (const std::string &inputPath, const std::string &basePath);
void compileWeapons (const std::string &inputPath, const std::string &basePath);
void compileFlingy  (const std::string &inputPath, const std::string &basePath);
void compileSprites (const std::string &inputPath, const std::string &basePath);
void compileImages  (const std::string &inputPath, const std::string &basePath);
void compileUpgrades(const std::string &inputPath, const std::string &basePath);
void compileTechdata(const std::string &inputPath, const std::string &basePath);
void compileSfxdata (const std::string &inputPath, const std::string &basePath);
void compilePortdata(const std::string &inputPath, const std::string &basePath);
void compileMapdata (const std::string &inputPath, const std::string &basePath);
void compileOrders  (const std::string &inputPath, const std::string &basePath);

void decompileUnits   (const std::string &inputPath);
void decompileWeapons (const std::string &inputPath);
void decompileFlingy  (const std::string &inputPath);
void decompileSprites (const std::string &inputPath);
void decompileImages  (const std::string &inputPath);
void decompileUpgrades(const std::string &inputPath);
void decompileTechdata(const std::string &inputPath);
void decompileSfxdata (const std::string &inputPath);
void decompilePortdata(const std::string &inputPath);
void decompileMapdata (const std::string &inputPath);
void decompileOrders  (const std::string &inputPath);

} //datcc
