#include "internal.h"
#include <iostream>

wchar_t clientName[] = { L"client.dll" };
wchar_t engineName[] = { L"engine.dll" };

extern IClient eClient;
extern IEngine eEngine;

IClient::IClient()
{
	pClientBase = GetModuleBaseAddress(clientName);
	pEngineBase = GetModuleBaseAddress(engineName);

	pLocalPlayer = pClientBase + ofs::st::dwEntityList;
	pEntityList = pClientBase + ofs::st::dwEntityList + 0x10;
	pClientState = *(uintptr_t*)(pEngineBase + ofs::st::dwClientState);

	maxPlayers = *reinterpret_cast<uint32_t*>(pClientState + ofs::st::dwClientState_MaxPlayer);
	aimSmooth = 10000;
	rcsSmooth = 10000;
	pEntity* lp = *reinterpret_cast<pEntity**>(pLocalPlayer);
	localPlayer = lp;
	updateVM();
}

void IClient::updateVM()
{
	memcpy(&viewMatrix, (PBYTE*)(pClientBase + ofs::st::dwViewMatrix), sizeof(viewMatrix));
}


IEngine::IEngine()
{
	width = 0;
	height = 0;

	viewAngle = (Vec3*)(eClient.pClientState + ofs::st::dwClientState_ViewAngles);
}
