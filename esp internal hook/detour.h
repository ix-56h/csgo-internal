#pragma once
#include <windows.h>

class detour
{
public:
	detour(char* src, char* dst, size_t len);
	~detour();
	bool hook();
	char* trampHook();
	bool unhook();
private:
	void* _stolenBytes;
	char* _src;
	char* _dst;
	size_t _len;
};