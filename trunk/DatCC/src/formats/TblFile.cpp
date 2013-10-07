#include "TblFile.h"
#include "../util.h"
#include <fstream>

namespace datcc {

TblFile statTxtTbl, imagesTbl, sfxdataTbl;

TblFile::~TblFile() {
  delete data;
}

int TblFile::loadFile(const std::string &fileName) {
  std::ifstream in(fileName.c_str(), std::ios::binary);
  if (in.fail()) return -1;

  dataSize = getFileSize(in);

  data = new BYTE[dataSize];
  in.read((char*)data, dataSize);
  if (in.fail()) return -1;
  in.close();

  stringCount = *((WORD*) data);
  stringOffsets = (WORD*) (data + 2);
  return 0;
}

const char* TblFile::getString(int index) const {
  static char invalidIndexMsg[] = "invalid string index";
  static char noneStr[] = "None";

  if (0 < index && index <= stringCount)
    return (char*) data + stringOffsets[index - 1];
  else if (index == 0)
    return noneStr;
  else
    return invalidIndexMsg;
}

size_t TblFile::getStringSize(int index) const {
  if (0 < index && index <= stringCount) {
    const char *strStart = getString(index);
    if (index == stringCount)
      return (char*) data + dataSize - strStart;
    else
      return getString(index + 1) - strStart;
  }
  else
    return strlen(getString(index));
}

const char* TblFile::getEscapedString(int index) const {
  const size_t stringSize = getStringSize(index);
  const char *string = getString(index);
  static char buffer[1000] = "";

  size_t pos = 0;
  for (size_t i = 0; i < stringSize; ++i) {
    if (iscntrl(string[i])) {
      char smallbuf[10];
      sprintf(smallbuf, "<%d>", (int)string[i]);
      strcpy(&buffer[pos], smallbuf);
      pos += strlen(smallbuf);
    }
    else
      buffer[pos++] = string[i];
  }
  buffer[pos] = '\0';

  return buffer;
}

} //datcc
