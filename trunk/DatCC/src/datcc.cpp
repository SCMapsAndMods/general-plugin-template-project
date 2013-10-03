#include "datcc.h"
#include "IniWriter.h"
#include "IniReader.h"
#include "data.h"
#include "util.h"
#include <iostream>
#include <fstream>

namespace datcc {

template <class DatT>
void compileDat(const std::string &inputIniPath) {
  std::cout << "Reading default data..." << std::endl;
  const std::string defaultDatPath = getCurrentProgramDir() + DefaultDat<DatT>::path;

  std::ifstream defaultDatStream(defaultDatPath.c_str(), std::ios::binary);
  if (defaultDatStream.fail()) {
    std::cerr << "Error: Cannot open default DAT (" << defaultDatPath << ")\n";
    return;
  }

  DatT dat;
  if (dat.getDataSize() != getFileSize(defaultDatStream)) {
    std::cerr << "Error: File size mismatch (" << defaultDatPath
              << " is " << getFileSize(defaultDatStream)
              << ", expected " << dat.getDataSize() << ")\n";
    return;
  }
  
  defaultDatStream.read((char*) dat.getData(), dat.getDataSize());
  if (defaultDatStream.fail()) {
    std::cerr << "Error: Failed reading in " << defaultDatPath << std::endl;
    return;
  }
  defaultDatStream.close();

  std::cout << "Reading from " << inputIniPath << "...\n";
  IniReader iniReader;
  if (0 > iniReader.loadFrom(inputIniPath)) {
    std::cerr << "Error: Could not read from " << inputIniPath << std::endl;
    return;
  }
  dat.processIni(iniReader);

  const std::string outputDatPath = getOutputDatPath(inputIniPath);
  std::cout << "Writing to " << outputDatPath << "...\n";

  std::ofstream outputDatStream(outputDatPath.c_str(), std::ios::binary | std::ios::trunc);
  if (outputDatStream.fail()) {
    std::cerr << "Error: Cannot open default DAT (" << defaultDatPath << ")\n";
    return;
  }

  outputDatStream.write((char*) dat.getData(), dat.getDataSize());
  if (outputDatStream.fail()) {
    std::cerr << "Error: Failed writing to " << defaultDatPath << std::endl;
    return;
  }

  outputDatStream.close();
}

template <class DatT>
void decompileDat(const std::string &inputDatPath_) {
  const bool useDefaultDat = (inputDatPath_ == ".");

  std::string inputDatPath;
  if (useDefaultDat)
    inputDatPath = getCurrentProgramDir() + DefaultDat<DatT>::path;
  else
    inputDatPath = inputDatPath_;
  std::cout << "Reading from " << inputDatPath << "...\n";

  std::ifstream inputDatStream(inputDatPath.c_str(), std::ios::binary);
  if (inputDatStream.fail()) {
    std::cerr << "Error: Cannot open " << inputDatPath << std::endl;
    return;
  }

  DatT dat;
  if (dat.getDataSize() != getFileSize(inputDatStream)) {
    std::cerr << "Error: File size mismatch (" << inputDatPath
              << " is " << getFileSize(inputDatStream)
              << ", expected " << dat.getDataSize() << ")\n";
    return;
  }

  inputDatStream.read((char*) dat.getData(), dat.getDataSize());
  if (inputDatStream.fail()) {
    std::cerr << "Error: Failed reading in " << inputDatPath << std::endl;
    return;
  }
  inputDatStream.close();

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

//-------- Compile functions --------//

void compileUnits(const std::string &inputPath) {
  compileDat<UnitsDat>(inputPath);
}

void compileWeapons(const std::string &inputPath) {
  compileDat<WeaponsDat>(inputPath);
}

void compileFlingy(const std::string &inputPath) {
  compileDat<FlingyDat>(inputPath);
}

void compileSprites(const std::string &inputPath) {
  compileDat<SpritesDat>(inputPath);
}

void compileImages  (const std::string &inputPath);

void compileUpgrades(const std::string &inputPath) {
  compileDat<UpgradesDat>(inputPath);
}

void compileTech    (const std::string &inputPath);
void compileOrders  (const std::string &inputPath);

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

void decompileImages  (const std::string &inputPath);

void decompileUpgrades(const std::string &inputPath) {
  decompileDat<UpgradesDat>(inputPath);
}

void decompileTech    (const std::string &inputPath);
void decompileOrders  (const std::string &inputPath);


} //datcc
