#pragma once
#include "internal.h"

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName);
bool WorldToScreen(Vec3 pos, Vec2& screen);
Vec3 GetBonePos(pEntity* ent, int bone);

Vec3 calcAngle(Vec3 src, Vec3 dst);
pEntity* getClosestEntity();
