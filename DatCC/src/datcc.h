#pragma once
#include <string>

namespace datcc {

void compileUnits   (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileWeapons (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileFlingy  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileSprites (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileImages  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileUpgrades(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileTechdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileSfxdata (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
//void compilePortdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
//void compileMapdata (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compileOrders  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);

void decompileUnits   (const std::string &inputPath, const std::string &outputPath);
void decompileWeapons (const std::string &inputPath, const std::string &outputPath);
void decompileFlingy  (const std::string &inputPath, const std::string &outputPath);
void decompileSprites (const std::string &inputPath, const std::string &outputPath);
void decompileImages  (const std::string &inputPath, const std::string &outputPath);
void decompileUpgrades(const std::string &inputPath, const std::string &outputPath);
void decompileTechdata(const std::string &inputPath, const std::string &outputPath);
void decompileSfxdata (const std::string &inputPath, const std::string &outputPath);
//void decompilePortdata(const std::string &inputPath, const std::string &outputPath);
//void decompileMapdata (const std::string &inputPath, const std::string &outputPath);
void decompileOrders  (const std::string &inputPath, const std::string &outputPath);

void compareUnits   (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareWeapons (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareFlingy  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareSprites (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareImages  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareUpgrades(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareTechdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareSfxdata (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
//void comparePortdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
//void compareMapdata (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareOrders  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);

} //datcc
