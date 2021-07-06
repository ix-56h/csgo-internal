#pragma once
#include "internal.h"

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName);
bool WorldToScreen(Vec3 pos, Vec2& screen);
Vec3 GetBonePos(pEntity* ent, int bone);