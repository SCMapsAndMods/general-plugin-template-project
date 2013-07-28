// This file contains the addresses to various functions and data structures
// within StarCraft.

#pragma once
#include "../definitions.h"
#include "../types.h"

/*
 *  These addresses are from the BWAPI/BWAI team, Poiuy_qwert,
 *  the EUDDB, BWHacks, and myself.  Thank you everyone :D .
 */

//Note: This namespace is named "offsets", not "offset", to avoid a clash with
//the x86 assembly keyword "OFFSET".
namespace offsets {

//For StarCraft v1.16.1
#if SC_VERSION == 1161

//Addresses for data in arr\*.dat files
const u32 UnitsDat      = 0x00513C30;  // 1.15.3, 1.16, 1.16.1
const u32 WeaponsDat    = 0x00513868;
const u32 FlingyDat     = 0x00515A38;
const u32 UpgradesDat   = 0x005136E0;
const u32 TechdataDat   = 0x005137D8;

//Addresses of commonly used arrays and data structures
const u32 Resources         = 0x0057F0F0;
const u32 PlayerTable       = 0x0057EEE0;
const u32 UnitDeaths        = 0x0058A364;  //++Taken from modified GPTP of xAngelSpiritx
const u32 UnitTable         = 0x0059CCA8;
const u32 LocationTable     = 0x0058DC60;  //++Taken from modified GPTP of xAngelSpiritx
const u32 ActionTable       = 0x00512800;  //++Taken from modified GPTP of xAngelSpiritx
const u32 BulletTable       = 0x0064B2E8;
const u32 FirstVisibleUnit  = 0x00628430; //Taken from BWAPI's Offsets.h
const u32 FirstHiddenUnit   = 0x006283EC; //Taken from BWAPI's Offsets.h
const u32 FirstScannerSweep = 0x006283F4; //Taken from BWAPI's Offsets.h
const u32 FirstBullet       = 0x0064DEC4; //Taken from BWAPI's Offsets.h
const u32 FirstPlayerUnit   = 0x006283F8;
const u32 ActiveTileArray   = 0x006D1260;
const u32 UnitOrderingX     = 0x0066FF78;
const u32 UnitOrderingY     = 0x006769B8;

//Addresses of useful SCBW internal variables (defined in scbwdata.h)
const u32 Is_Game_Paused      = 0x006509C4;
const u32 Is_Brood_War        = 0x0058F440;
const u32 Cheat_State         = 0x006D5A6C;
const u32 MapSize             = 0x0057F1D4;
const u32 UnitOrderingCount   = 0x0066FF74;
const u32 MaxUnitWidth        = 0x006BEE68;
const u32 MaxUnitHeight       = 0x006BB930;

//Addresses of useful SCBW internal functions (defined in api.h)
const u32 Func_PrintText            = 0x0048CD30;
const u32 Func_DoWeaponDamage       = 0x00479930;
const u32 Func_RemoveOverlays       = 0x004E5CF0;
const u32 Func_CreateOverlay        = 0x00498EA0;
const u32 Func_SetUnitHp            = 0x00467340;
const u32 Func_DamageUnitHp         = 0x004797B0;
const u32 Func_FireUnitWeapon       = 0x00479C90;
const u32 Func_PlayIscriptAnim      = 0x004D8470; //AKA playImageIscript()
const u32 Func_CreateUnitAtPos      = 0x004CD360; //AKA createUnitXY()
const u32 Func_OrderToUnit          = 0x004752B0; //AKA AssignOrderWithTarget(); Primarily for use with orderNewUnitToRally(), but may have other uses.
const u32 Func_OrderToPos           = 0x00475260; //AKA orderTarget(); Primarily for use with orderNewUnitToRally(), but may have other uses.
const u32 Func_CanBeEnteredBy       = 0x004E6E00; //AKA CanEnterTransport()
const u32 Func_HasPathToPos         = 0x0049CB60; //AKA unitHasPathToDest()
const u32 Func_HasPathToTarget      = 0x0049CBB0; //AKA unitHasPathToUnit()
const u32 Func_RemoveUnit           = 0x00475710; //AKA orders_SelfDestructing()
const u32 Func_UpdateSpeed          = 0x00454310;
const u32 Func_PlaySound            = 0x0048ED50;
const u32 Func_GetGroundHeightAtPos = 0x004BD0F0;
const u32 Func_RemoveStasisField    = 0x004F62D0;
const u32 Func_RemoveLockdown       = 0x00454D90;
const u32 Func_RemoveMaelstrom      = 0x00454D20;

//For StarCraft v1.15.1
#elif SC_VERSION == 1151

const u32 GameStart        = 0x004C9691;
const u32 GameStartBack      = GameStart + 5;
const u32 GameStartTarget      = 0x00417C10;
const u32 GameEnd          = 0x004EE5C3;
const u32 GameEndBack        = GameEnd + 5;
const u32 GameEndTarget      = 0x00416D80;
const u32 NextLogicFrame      = 0x004D936E;
const u32 NextLogicFrameBack    = NextLogicFrame + 5;
const u32 NextLogicFrameTarget  = 0x00488420;

//Addresses for data in arr\*.dat files
const u32 UnitsDat    = 0x00513C30;  // 1.15.3, 1.16, 1.16.1
const u32 WeaponsDat    = 0x00513868;
const u32 FlingyDat    = 0x00515A38;
const u32 UpgradesDat    = 0x005136E0;
const u32 TechdataDat    = 0x005137D8;

const u32 Resources = 0x0057F0D8;
const u32 PlayerTable = 0x0057EEC0;
const u32 UnitDeaths = 0x0058A34C;
const u32 UnitTable = 0x0059CC90;
const u32 LocationTable = 0x0058DC48;
const u32 ActionTable = 0x00512800;

//Addresses of useful SCBW internal constants (defined in scbwdata.h)
const u32 Game_Is_Paused    = 0x006509AC; //Haven't checked

const u32 NameSpoofer = 0x19044EA8;
const u32 HostHack = 0x00596870;
const u32 HostName = 0x00596814; //for auto-spoof to host name
const u32 CurrentPlayerSlot = 0x00512684;
const u32 PrintText = 0x48CE60;
const u32 PrintTextAnywhere = 0x4202A0;
const u32 FontSize = 0x41FB20;

enum FontSize
{
  Small = 0x6CE0DC,
  Normal = 0x6CE0E0,
  Large = 0x6CE0E4,
  Huge = 0x6CE0E8
};

const u32 GameText = 0x4F2EC0; // Send Messages 1.15.1
const u32 HostFunc = 0x4C46D0; // host function for banning, open, close, computer, 1.15.1
const u32 HostHack2 = 0x4512D8; // lets you be host even if ur not 1.15.1
const u32 LbyJMPb = 0x4B9583; // offset to jump back to lobby handle text 1.15.1
const u32 GameJMPb = 0x4F2FFC; // Offset to jump back to from game text handle. 1.15.1
const u32 CLobbySendText = 0x4B8BB0; // Client: sends text to lobby 1.15.1

const u32 HealthBarHack = 0x004E5E62;
  /*
   * Original Poster:
   * "NOPing twice makes Starcraft unit's Health bars
   * display without having to click on them.
   * Like Warcraft 3's Alt button."
   *
   * Me:
   * "I've found that NOPing this address in OllyDbg
   * causes only the first ~12 units clicked to
   * ALWAYS display their health bar.  I'm not sure
   * if the OP meant to NOP the whole function (which
   * is about 6 lines) or to just NOP the address.
   * Either way, test this one with caution."
   *
   * BTW, NOPing means filling the address with WORD 0x90 (?).
   *
   */

//SC_VERSION == 1151
#endif

} //offsets
