#include "datcc.h"
#include "IniWriter.h"
#include "IniReader.h"
#include "IniComparator.h"
#include "dat_io.h"
#include "data.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>

namespace datcc {

template <class DatT>
void compileDat(const std::string &inputIniPath, const std::string &basePath) {
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

  const std::string outputDatPath = getOutputDatPath(inputIniPath);
  std::cout << "Writing to " << outputDatPath << "...\n";
  if (saveDat(dat, outputDatPath)) return;
}

template <class DatT>
void decompileDat(const std::string &inputDatPath_) {
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
  if (useDefaultDat)
    outputIniPath = getOutputIniPath(std::string(DefaultDat<DatT>::path).erase(0, 5)); //Erase "data/"
  else
    outputIniPath = getOutputIniPath(inputDatPath);
  std::cout << "Writing to " << outputIniPath << "...\n";
  if (0 > iniExporter.saveTo(outputIniPath)) {
    std::cerr << "Error: Could not save to " << outputIniPath << std::endl;
    return;
  }
}

template <class DatT>
void compareDat(const std::string &inputDatPath, const std::string &basePath) {
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

  std::string outputIniPath = getOutputIniPath(inputDatPath);
  std::cout << "Decompiling differences to " << outputIniPath << "...\n";
  if (0 > iniComparator.saveTo(outputIniPath)) {
    std::cerr << "Error: Could not save to " << outputIniPath << std::endl;
    return;
  }
}

//-------- Compile functions --------//

void compileUnits(const std::string &inputPath, const std::string &basePath) {
  compileDat<UnitsDat>(inputPath, basePath);
}

void compileWeapons(const std::string &inputPath, const std::string &basePath) {
  compileDat<WeaponsDat>(inputPath, basePath);
}

void compileFlingy(const std::string &inputPath, const std::string &basePath) {
  compileDat<FlingyDat>(inputPath, basePath);
}

void compileSprites(const std::string &inputPath, const std::string &basePath) {
  compileDat<SpritesDat>(inputPath, basePath);
}

void compileImages(const std::string &inputPath, const std::string &basePath) {
  compileDat<ImagesDat>(inputPath, basePath);
}

void compileUpgrades(const std::string &inputPath, const std::string &basePath) {
  compileDat<UpgradesDat>(inputPath, basePath);
}

void compileTechdata(const std::string &inputPath, const std::string &basePath) {
  compileDat<TechdataDat>(inputPath, basePath);
}

void compileOrders  (const std::string &inputPath, const std::string &basePath);

//-------- Decompile functions --------//

void decompileUnits(const std::string &inputPath) {
  decompileDat<UnitsDat>(inputPath);
}

void decompileWeapons(const std::string &inputPath) {
  decompileDat<WeaponsDat>(inputPath);
}

void decompileFlingy(const std::string &inputPath) {
  decompileDat<FlingyDat>(inputPath);
}

void decompileSprites(const std::string &inputPath) {
  decompileDat<SpritesDat>(inputPath);
}

void decompileImages(const std::string &inputPath) {
  decompileDat<ImagesDat>(inputPath);
}

void decompileUpgrades(const std::string &inputPath) {
  decompileDat<UpgradesDat>(inputPath);
}

void decompileTechdata(const std::string &inputPath) {
  decompileDat<TechdataDat>(inputPath);
}

void decompileOrders  (const std::string &inputPath);

//-------- Compare functions --------//

void compareUnits   (const std::string &inputPath, const std::string &basePath) {
  compareDat<UnitsDat>(inputPath, basePath);
}

void compareWeapons (const std::string &inputPath, const std::string &basePath) {
  compareDat<WeaponsDat>(inputPath, basePath);
}

void compareFlingy  (const std::string &inputPath, const std::string &basePath) {
  compareDat<FlingyDat>(inputPath, basePath);
}

void compareSprites (const std::string &inputPath, const std::string &basePath) {
  compareDat<SpritesDat>(inputPath, basePath);
}

void compareImages  (const std::string &inputPath, const std::string &basePath) {
  compareDat<ImagesDat>(inputPath, basePath);
}

void compareUpgrades(const std::string &inputPath, const std::string &basePath) {
  compareDat<UpgradesDat>(inputPath, basePath);
}

void compareTechdata(const std::string &inputPath, const std::string &basePath) {
  compareDat<TechdataDat>(inputPath, basePath);
}

void compareSfxdata (const std::string &inputPath, const std::string &basePath);
void comparePortdata(const std::string &inputPath, const std::string &basePath);
void compareMapdata (const std::string &inputPath, const std::string &basePath);
void compareOrders  (const std::string &inputPath, const std::string &basePath);

} //datcc
