#ifndef INTERNAL_H
#define INTERNAL_H
#include "offsets.h"

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName);
//Vec3 bones(pEntity* ent, int boneID);

struct view_matrix_t {
	float matrix[16];
};
#endif#pragma once
