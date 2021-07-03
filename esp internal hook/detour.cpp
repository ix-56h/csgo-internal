#include "detour.h"

/*
	class : detour
	Function : Constructor
	Purpose : Init private variables
*/
detour::detour(void *src, void* dst, size_t len) : _src(src), _dst(dst), _len(len)
{
	_stolenBytes = new BYTE[len];
}

/*
	class : detour
	Function : Destructor
	Purpose : Unhook if hooked and delete stolenBytes
*/
detour::~detour()
{
	//if hooked
	//	unhook();
	delete[]_stolenBytes;
}
/*
	class : detour
	Function : hook
	Purpose : Process to the detour, rewrite n bytes at _src address and put the jmp
*/
bool detour::hook()
{
	// check if the minimum len is respected, 5 = minimum bytes for jmp
	if (_len < 5)
		return false;

	DWORD protect;
	VirtualProtect(_src, _len, PAGE_EXECUTE_READWRITE, &protect);
	
	//TODO : memcopy stolen bytes used for trampoline
	memcpy(_stolenBytes, _src, _len);

	// safe : NOPing (basic detour good practice)
	memset(_src, 0x90, _len);

	//VirtualProtect(_src, _len, protect, &protect);
	//return false;

	DWORD relativeAddr = ((DWORD)_dst - (DWORD)_src) - _len;
	
	// 0x90 = NOP, 0xE9 = jmp
	//set jmp
	*(BYTE*)_src = 0xE9;
	*(DWORD*)((DWORD)_src + 1) = relativeAddr;

	VirtualProtect(_src, _len, protect, &protect);

	return true;
}

/*
	class : detour
	Function : unhook
	Purpose : Restore stolenBytes
*/
bool detour::unhook()
{
	return false;
}