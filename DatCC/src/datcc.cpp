#include "datcc.h"
#include "ini_processors/IniWriter.h"
#include "ini_processors/IniReader.h"
#include "ini_processors/IniComparator.h"
#include "dat_io.h"
#include "data.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>

namespace datcc {

template <class DatT>
void compileDat(const std::string &inputIniPath, const std::string &outputDatPath_, const std::string &basePath) {
  std::string loadBasePath;
  const bool useDefaultDat = (basePath == ".");

  if (useDefaultDat) {
    loadBasePath = getCurrentProgramDir() + DefaultDat<DatT>::path;
    std::cout << "Reading default DAT..." << std::endl;
  }
  else {
    loadBasePath = basePath;
    std::cout << "Reading base DAT from " << loadBasePath << "...\n";
  }

  DatT dat;
  if (loadDat(dat, loadBasePath)) return;

  std::cout << "Reading from " << inputIniPath << "...\n";
  IniReader iniReader;
  if (0 > iniReader.loadFrom(inputIniPath)) {
    std::cerr << "Error: Could not read from " << inputIniPath << std::endl;
    return;
  }
  dat.processIni(iniReader);

  std::string outputDatPath;
  if (outputDatPath_ == "" )
    outputDatPath = getOutputDatPath(inputIniPath);
  else
    outputDatPath = outputDatPath_;

  std::cout << "Writing to " << outputDatPath << "...\n";
  if (saveDat(dat, outputDatPath)) return;
}

template <class DatT>
void decompileDat(const std::string &inputDatPath_, const std::string &outputIniPath_) {
  const bool useDefaultDat = (inputDatPath_ == ".");

  std::string inputDatPath;
  if (useDefaultDat) {
    inputDatPath = getCurrentProgramDir() + DefaultDat<DatT>::path;
    std::cout << "Using default DAT file (" << inputDatPath << ")\n";
  }
  else {
    inputDatPath = inputDatPath_;
    std::cout << "Reading from " << inputDatPath << "...\n";
  }

  DatT dat;
  if (loadDat(dat, inputDatPath)) return;

  std::cout << "Converting to INI format...\n";
  IniWriter iniExporter;
  dat.processIni(iniExporter);

  std::string outputIniPath;
  if (outputIniPath_ == "") {
    if (useDefaultDat)
      outputIniPath = getOutputIniPath(std::string(DefaultDat<DatT>::path).erase(0, 5)); //Erase "data/"
    else
      outputIniPath = getOutputIniPath(inputDatPath);
  }
  else
    outputIniPath = outputIniPath_;

  std::cout << "Writing to " << outputIniPath << "...\n";
  if (0 > iniExporter.saveTo(outputIniPath)) {
    std::cerr << "Error: Could not save to " << outputIniPath << std::endl;
    return;
  }
}

template <class DatT>
void compareDat(const std::string &inputDatPath, const std::string &outputIniPath_, const std::string &basePath) {
  std::string loadBasePath;
  const bool useDefaultDat = (basePath == ".");

  if (useDefaultDat) {
    loadBasePath = getCurrentProgramDir() + DefaultDat<DatT>::path;
    std::cout << "Reading default DAT..." << std::endl;
  }
  else {
    loadBasePath = basePath;
    std::cout << "Reading base DAT from " << loadBasePath << "...\n";
  }

  DatT baseDat;
  if (loadDat(baseDat, loadBasePath)) return;

  DatT dat;
  std::cout << "Reading from " << inputDatPath << "...\n";
  if (loadDat(dat, inputDatPath)) return;

  IniComparator iniComparator;
  iniComparator.compare(dat, baseDat);

  std::string outputIniPath;
  if (outputIniPath_ == "")
    outputIniPath = getOutputIniPath(inputDatPath);
  else
    outputIniPath = outputIniPath_;

  std::cout << "Decompiling differences to " << outputIniPath << "...\n";
  if (0 > iniComparator.saveTo(outputIniPath)) {
    std::cerr << "Error: Could not save to " << outputIniPath << std::endl;
    return;
  }
}

//-------- Compile functions --------//

void compileUnits(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<UnitsDat>(inputPath, outputPath, basePath);
}

void compileWeapons(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<WeaponsDat>(inputPath, outputPath, basePath);
}

void compileFlingy(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<FlingyDat>(inputPath, outputPath, basePath);
}

void compileSprites(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<SpritesDat>(inputPath, outputPath, basePath);
}

void compileImages(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<ImagesDat>(inputPath, outputPath, basePath);
}

void compileUpgrades(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<UpgradesDat>(inputPath, outputPath, basePath);
}

void compileTechdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compileDat<TechdataDat>(inputPath, outputPath, basePath);
}

void compileOrders  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);

//-------- Decompile functions --------//

void decompileUnits(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<UnitsDat>(inputPath, outputPath);
}

void decompileWeapons(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<WeaponsDat>(inputPath, outputPath);
}

void decompileFlingy(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<FlingyDat>(inputPath, outputPath);
}

void decompileSprites(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<SpritesDat>(inputPath, outputPath);
}

void decompileImages(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<ImagesDat>(inputPath, outputPath);
}

void decompileUpgrades(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<UpgradesDat>(inputPath, outputPath);
}

void decompileTechdata(const std::string &inputPath, const std::string &outputPath) {
  decompileDat<TechdataDat>(inputPath, outputPath);
}

void decompileOrders  (const std::string &inputPath, const std::string &outputPath);

//-------- Compare functions --------//

void compareUnits   (const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<UnitsDat>(inputPath, outputPath, basePath);
}

void compareWeapons (const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<WeaponsDat>(inputPath, outputPath, basePath);
}

void compareFlingy  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<FlingyDat>(inputPath, outputPath, basePath);
}

void compareSprites (const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<SpritesDat>(inputPath, outputPath, basePath);
}

void compareImages  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<ImagesDat>(inputPath, outputPath, basePath);
}

void compareUpgrades(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<UpgradesDat>(inputPath, outputPath, basePath);
}

void compareTechdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath) {
  compareDat<TechdataDat>(inputPath, outputPath, basePath);
}

void compareSfxdata (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void comparePortdata(const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareMapdata (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);
void compareOrders  (const std::string &inputPath, const std::string &outputPath, const std::string &basePath);

} //datcc
