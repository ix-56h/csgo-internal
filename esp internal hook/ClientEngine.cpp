#include "internal.h"
#include <iostream>

wchar_t clientName[] = { L"client.dll" };
wchar_t engineName[] = { L"engine.dll" };

extern IClient eClient;
//extern IEngine eEngine;

IClient::IClient()
{
	pClientBase = GetModuleBaseAddress(clientName);
	pEngineBase = GetModuleBaseAddress(engineName);

	pLocalPlayer = pClientBase + hazedumper::signatures::dwEntityList;
	pEntityList = pClientBase + hazedumper::signatures::dwEntityList + 0x10;
	pClientState = *(uintptr_t*)(pEngineBase + hazedumper::signatures::dwClientState);

	maxPlayers = *reinterpret_cast<uint32_t*>(pClientState + hazedumper::signatures::dwClientState_MaxPlayer);

	pEntity* lp = *reinterpret_cast<pEntity**>(pLocalPlayer);
	localPlayer = lp;
	updateVM();
}

void IClient::updateVM()
{
	memcpy(&viewMatrix, (PBYTE*)(pClientBase + hazedumper::signatures::dwViewMatrix), sizeof(viewMatrix));
}

/*
IEngine::IEngine()
{
	width = *reinterpret_cast<int*>(eClient.pEngineBase + 0x38AE5CC);
	height = *reinterpret_cast<int*>(eClient.pEngineBase + 0x38AE5C0);

	viewAngle = (Vec3*)(eClient.pClientState + hazedumper::signatures::dwClientState_ViewAngles);
}

void IEngine::getScreenSize()
{
	width = *reinterpret_cast<uint32_t*>(eClient.pEngineBase + 0x38AE5CC);
	height = *reinterpret_cast<uint32_t*>(eClient.pEngineBase + 0x38AE5C0);
}
*/