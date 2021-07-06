#include "detour.h"

/*
	class : detour
	Function : Constructor
	Purpose : Init private variables
*/
detour::detour(char* src, char* dst, size_t len) : _src(src), _dst(dst), _len(len)
{
	_stolenBytes = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (len >= 5 && _stolenBytes != NULL)
		memcpy(_stolenBytes, _src, _len);
}

/*
	class : detour
	Function : Destructor
	Purpose : Unhook if hooked and delete stolenBytes
*/
detour::~detour()
{
	if (_stolenBytes != NULL)
		VirtualFree(_stolenBytes, 0, MEM_RELEASE);
}

/*
	class : detour
	Function : hook
	Purpose : Process to the detour, rewrite n bytes at _src address and put the jmp
*/
bool detour::hook()
{
	// check if the minimum len is respected, 5 = minimum bytes for jmp
	if (_len < 5 || _stolenBytes == NULL)
		return false;
	DWORD protect;
	VirtualProtect(_src, _len, PAGE_EXECUTE_READWRITE, &protect);

	// safe : NOPing (basic detour good practice)
	memset(_src, 0x90, _len);

	intptr_t relativeAddr = ((intptr_t)_dst - (intptr_t)_src) - 5;

	// 0x90 = NOP, 0xE9 = jmp
	//set jmp
	*(BYTE*)_src = 0xE9;
	*(intptr_t*)((intptr_t)_src + 1) = relativeAddr;

	VirtualProtect(_src, _len, protect, &protect);

	return true;
}

/*
	class : detour
	Function : trampHook
	Purpose : Setup a gateway with stolen bytes
*/
char* detour::trampHook()
{
	// check if the minimum len is respected, 5 = minimum bytes for jmp
	if (_len < 5 || _stolenBytes == NULL)
		return nullptr;
	//memcopy stolen bytes used for trampoline (_stolenBytes is our gateway)
	memcpy(_stolenBytes, _src, _len);

	intptr_t gatewayRelativeAddr = ((intptr_t)_src - (intptr_t)_stolenBytes) - 5;
	
	// 0x90 = NOP, 0xE9 = jmp
	//set jmp AFTER the rewritten bytes
	*(char*)((intptr_t)_stolenBytes + _len) = 0xE9;
	*(intptr_t*)((intptr_t)_stolenBytes + _len + 1) = gatewayRelativeAddr;

	if (hook() == false)
	{
		VirtualFree(_stolenBytes, 0, MEM_RELEASE);
		return nullptr;
	}
	
	return (char*)_stolenBytes;
}

/*
	class : detour
	Function : unhook
	Purpose : Restore stolenBytes
*/
bool detour::unhook()
{
	DWORD protect;
	VirtualProtect(_src, _len, PAGE_EXECUTE_READWRITE, &protect);
	memcpy(_src, _stolenBytes, _len);
	VirtualProtect(_src, _len, protect, &protect);

	return false;
}