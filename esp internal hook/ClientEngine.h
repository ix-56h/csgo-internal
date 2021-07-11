#pragma once
#include "internal.h"

class IClient
{
public:
	IClient();
	uintptr_t pClientBase;
	uintptr_t pEngineBase;

	uintptr_t pLocalPlayer;
	uintptr_t pEntityList;
	uintptr_t pClientState;

	uint32_t  maxPlayers;

	pEntity* localPlayer;
	uint32_t  aimSmooth;
	uint32_t  rcsSmooth;

	float viewMatrix[16];
	void updateVM();
};

class IEngine
{
public:
	IEngine();

	uint32_t width;
	uint32_t height;
	Vec3* viewAngle;
};