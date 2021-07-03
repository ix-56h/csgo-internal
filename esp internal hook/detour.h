#pragma once
#include <windows.h>

class detour
{
public:
	detour(void* src, void* dst, size_t len);
	~detour();
	bool hook();
	bool unhook();
private:
	BYTE* _stolenBytes;
	void* _src;
	void* _dst;
	size_t _len;
};