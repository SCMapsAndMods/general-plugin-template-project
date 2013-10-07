#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "util.h"

namespace datcc {

/// Loads DAT file from the given path. If something fails, prints error 
/// messages and returns a nonzero value.
template <class DatT>
int loadDat(DatT &dat, const std::string &loadPath);

/// Saves DAT file to the given path. If something fails, prints error messages
/// and returns a nonzero value.
template <class DatT>
int saveDat(DatT &dat, const std::string &savePath);


//-------- Function template definitions --------//

template <class DatT>
int loadDat(DatT &dat, const std::string &loadPath) {
  std::ifstream inputDatStream(loadPath.c_str(), std::ios::binary);

  if (inputDatStream.fail()) {
    std::cerr << "Error: Cannot load DAT file (" << loadPath << ")\n";
    return 1;
  }

  if (dat.getDataSize() != getFileSize(inputDatStream)) {
    std::cerr << "Error: File size mismatch (" << loadPath
              << " is " << getFileSize(inputDatStream)
              << " bytes, expected " << dat.getDataSize() << ")\n";
    return 2;
  }

  inputDatStream.read((char*) dat.getData(), dat.getDataSize());
  if (inputDatStream.fail()) {
    std::cerr << "Error: Failed reading DAT file (" << loadPath << ")\n";
    return 3;
  }

  return 0;
}

template <class DatT>
int saveDat(DatT &dat, const std::string &savePath) {
  std::ofstream outputDatStream(savePath.c_str(), std::ios::binary | std::ios::trunc);

  if (outputDatStream.fail()) {
    std::cerr << "Error: Cannot open DAT file for writing (" << savePath << ")\n";
    return 1;
  }

  outputDatStream.write((char*) dat.getData(), dat.getDataSize());
  if (outputDatStream.fail()) {
    std::cerr << "Error: Failed writing DAT file (" << savePath << ")\n";
    return 3;
  }

  return 0;
}

} //datcc
