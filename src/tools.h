//This header contains several functions used internally by GPTP.

#pragma once
#include <windows.h>
#include "types.h"

/// Injects a JMP to [target] at the [position], optionally overwriting [nops]
/// bytes with NOPs.
///
/// Injects a relative JMP (0xE9) instruction to the [target] address at the
/// given memory [position]. If [nops] is specified, also overwrites additional
/// bytes of memory with NOP (0x90) instructions. While this is technically
/// unnecessary, it is included here for completeness.
/// A hook function for JmpPatch() should usually end with a JMP instruction to
/// where the control of flow should return. This means you can manually control
/// where the function jumps to. Should you wish to merely return to where the
/// was injected, consider using CallPatch().
///
/// @param  target    The address of the function to jump to.
/// @param  position  The part of memory where the JMP instruction is injected.
/// @param  nops      Optional. The number of additional bytes to write NOPs.
void jmpPatch(const void* target, void* position, unsigned int nops = 0);
inline void jmpPatch(const void* target, DWORD position, unsigned int nops = 0) {
  jmpPatch(target, (void*)position, nops);
}

/// Injects a CALL to [target] at the [position], optionally overwriting [nops]
/// bytes with NOPs.
///
/// Injects a relative CALL (0xE8) instruction to the [target] address at the
/// given memory [position]. If [nops] is specified, also overwrites additional
/// bytes of memory with NOP (0x90) instructions. This is useful when you are
/// overwriting an instruction sequence that is longer than the size of the CALL
/// instruction (5 bytes).
/// A hook function for CallPatch() should usually end with a RET instruction,
/// which returns the control of flow to where the function was injected. This
/// is preferable when you want to hook a single function to multiple positions,
/// since there is no need to explicitly specify where to JMP back every time.
///
/// @param  target    The address of the function to call.
/// @param  position  The part of memory where the CALL instruction is injected.
/// @param  nops      Optional. The number of additional bytes to write NOPs.
void callPatch(const void* target, void* position, unsigned int nops = 0);
inline void callPatch(const void* target, DWORD position, unsigned int nops = 0) {
  callPatch(target, (void*)position, nops);
}

/// Patches the specified address with a 4-byte value.
void memoryPatch4(void* const address, const u32 value);

/// Patches the specified address with a 2-byte value.
void memoryPatch2(void* const address, const u16 value);

/// Patches the specified address with a 1-byte value.
void memoryPatch1(void* const address, const u8 value);

/// Inject an array of bytes, using the given length.
void memoryPatch(void* const address, const u8* data, const size_t length);

/// Displays a simple non-modal message box.
inline void mBox(const char* str) {
	MessageBox(0, str, "!", 0);
}
