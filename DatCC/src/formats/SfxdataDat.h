#include "DatInterface.h"
#include <sstream>
#include "TblFile.h"
#include "../flags.h"

namespace datcc {

//Based on http://modcrafters.com/wiki/index.php?title=Sfxdata.dat

const int SOUND_TYPE_COUNT = 1144;

#pragma pack(1)

struct SfxdataDatFile {
  DWORD soundFile [SOUND_TYPE_COUNT];
  BYTE  flags1    [SOUND_TYPE_COUNT];
  BYTE  flags2    [SOUND_TYPE_COUNT];
  WORD  race      [SOUND_TYPE_COUNT];
  BYTE  muteVolume[SOUND_TYPE_COUNT];
};

#pragma pack()

C_ASSERT(sizeof(SfxdataDatFile) == 10296);

class SfxdataDat {
  public:
    void* getData() { return &data; }
    const void* getData() const { return &data; }
    int getDataSize() const { return sizeof(data); }

    template <class IniProcT>
    void processIni(IniProcT &iniProc);
  private:
    SfxdataDatFile data;
};

template <class IniProcT>
void SfxdataDat::processIni(IniProcT &iniProc) {
  for (int soundId = 0; soundId < SOUND_TYPE_COUNT; ++soundId) {
    std::ostringstream sectionName;
    sectionName << "Sound #" << soundId;
    iniProc.setSection(sectionName.str(), "");

    iniProc.process(data.soundFile[soundId],    "Sound File", makeSfxdataTblComment);
    iniProc.process(data.flags1[soundId],  "Flags 1", unknownFlags8);
    iniProc.process(data.flags2[soundId],  "Flags 2", unknownFlags8);
    iniProc.process(data.race[soundId],         "Race");
    iniProc.process(data.muteVolume[soundId],   "Mute Volume %");
  }
}

} //datcc
