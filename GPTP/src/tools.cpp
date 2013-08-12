#include "tools.h"

//Injects a relative CALL to [target] at the [position].
//Original function from BWAPI by Kovarex; Modified by pastelmind
void callPatch(const void* target, void* position, unsigned int nops) {
	DWORD oldProt = 0;
	VirtualProtect(position, 5 + nops, PAGE_EXECUTE_READWRITE, &oldProt);
  *(BYTE*)position = 0xE8;  //Relative CALL instruction
	s32 address = (s32)target - (s32)position - 5;
  *(s32*)((DWORD)position + 1) = address; //Inject relative address
	for (unsigned int i = 0; i < nops; ++i)
		*(BYTE*)((DWORD)position + 5 + i) = 0x90; //NOP instruction
	VirtualProtect(position, 5 + nops, oldProt, &oldProt);
}

//Injects a relative JMP to [target] at the [position].
//Original function from BWAPI by Kovarex; Modified by pastelmind
void jmpPatch(const void* target, void* position, unsigned int nops) {
	DWORD oldProt = 0;
	VirtualProtect(position, 5 + nops, PAGE_EXECUTE_READWRITE, &oldProt);
	*(BYTE*)position = 0xE9;  //Relative JMP instruction
	s32 address = (s32)target - (s32)position - 5;
  *(s32*)((DWORD)position + 1) = address; //Inject relative address
	for (unsigned int i = 0; i < nops; ++i)
		*(BYTE*)((DWORD)position + 5 + i) = 0x90; //NOP instruction
	VirtualProtect(position, 5 + nops, oldProt, &oldProt);
}

//Patches the specified address with a 4-byte value.
void memoryPatch4(void* const address, const u32 value) {
	DWORD oldProt = 0;
	VirtualProtect(address, sizeof(u32), PAGE_EXECUTE_READWRITE, &oldProt);
	*(u32*)address = value;
	VirtualProtect(address, sizeof(u32), oldProt, &oldProt);
}

//Patches the specified address with a 2-byte value.
void memoryPatch2(void* const address, const u16 value) {
	DWORD oldProt = 0;
	VirtualProtect(address, sizeof(u16), PAGE_EXECUTE_READWRITE, &oldProt);
	*(u16*)address = value;
	VirtualProtect(address, sizeof(u16), oldProt, &oldProt);
}

//Patches the specified address with a 1-byte value.
void memoryPatch1(void* const address, const u8 value) {
	DWORD oldProt = 0;
	VirtualProtect(address, sizeof(u8), PAGE_EXECUTE_READWRITE, &oldProt);
	*(u8*)address = value;
	VirtualProtect(address, sizeof(u8), oldProt, &oldProt);
}

//Inject an array of bytes, using the given length.
void memoryPatch(void* const address, const u8* data, const size_t length) {
	DWORD oldProt = 0;
	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProt);
	memcpy(address, data, length);
	VirtualProtect(address, length, oldProt, &oldProt);
}
