#include "TblFile.h"
#include "../util.h"
#include <fstream>
#include <cassert>

TblFile statTxtTbl;

TblFile::~TblFile() {
  delete data;
}

int TblFile::loadFile(const char *fileName) {
  std::ifstream in(fileName, std::ios::binary);
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
  assert(0 <= index && index < stringCount);
  return (char*) data + stringOffsets[index];
}

int TblFile::getStringSize(int index) const {
  assert(0 <= index && index < stringCount);
  const char *strStart = getString(index);
  if (index == stringCount - 1)
    return (char*) data + dataSize - strStart;
  else
    return getString(index + 1) - strStart;
}

const char* TblFile::getEscapedString(int index) const {
  const int stringSize = getStringSize(index);
  const char *string = getString(index);
  static char buffer[1000] = "";

  int pos = 0;
  for (int i = 0; i < stringSize; ++i) {
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
