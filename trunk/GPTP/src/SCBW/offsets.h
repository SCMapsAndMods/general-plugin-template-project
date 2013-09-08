// This file contains the addresses to various functions within StarCraft. All
// offsets are for StarCraft: Brood War 1.16.1.

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

//Addresses of useful SCBW internal functions (defined in api.h)
const u32 Func_PrintText            = 0x0048CD30;
const u32 Func_DoWeaponDamage       = 0x00479930; //Note: Also used by the hook in do_weapon_damage.h
const u32 Func_RemoveOverlays       = 0x004E5CF0;
const u32 Func_CreateOverlay        = 0x00498EA0;
const u32 Func_SetUnitHp            = 0x00467340;
const u32 Func_DamageUnitHp         = 0x004797B0;
const u32 Func_FireUnitWeapon       = 0x00479C90;
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

} //offsets
