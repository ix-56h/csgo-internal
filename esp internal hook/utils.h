#pragma once
#include "internal.h"

struct threadShare
{
	uint32_t* smooth;
	bool* run;
};

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName);
bool WorldToScreen(Vec3 pos, Vec2& screen);
Vec3 GetBonePos(pEntity* ent, int bone);

bool betterTarget(Vec3 src, pEntity* entity, float* curClosestDistance, Vec3* curClosestAngle);

DWORD WINAPI aimbotTH(void *params);
DWORD WINAPI aimbotFOV(void* params);

Vec3 calcAngle(Vec3 src, Vec3 dst);
pEntity* getClosestEntity();
pEntity* getClosestEntityFOV();
DWORD WINAPI RCS(void* params);
