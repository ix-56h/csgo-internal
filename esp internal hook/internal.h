#ifndef INTERNAL_H
#define INTERNAL_H
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//include
#include <d3d9.h>
#include <d3dx9.h>

#include "offsets.h"

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName);
//Vec3 bones(pEntity* ent, int boneID);

struct view_matrix_t {
	float matrix[16];
};
#endif#pragma once
