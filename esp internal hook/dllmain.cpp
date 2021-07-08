// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "internal.h"
#include "D3Device.h"
#include "VMT_hook.h"
#include "detour.h"
#include <windows.h>
#include <iostream>

IClient eClient;
IEngine eEngine;
bool esp = false, aimbot = false, aimbotF = false;

LPDIRECT3DDEVICE9 gDevice;
// typedef the function prototype of EndScene
//typedef HRESULT APIENTRY EndScene(LPDIRECT3DDEVICE9);
// Declare a pointer on this function prototype
//EndScene *oEndScene = NULL;

typedef HRESULT(__stdcall* _EndScene)(LPDIRECT3DDEVICE9 pDevice);
_EndScene oEndScene = nullptr;

//void hkFunc(void) { void }
HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
    if (!gDevice)
    {
        gDevice = pDevice;
        return oEndScene(gDevice);
    }
    if (esp)
    {
        for (size_t i = 0; i < eClient.maxPlayers; i++)
        {
            pEntity* entity = *reinterpret_cast<pEntity**>(eClient.pEntityList + (i * 0x10));
            if (!entity || eClient.localPlayer->iTeamNum == entity->iTeamNum || entity->isDormant || entity == eClient.localPlayer)
                continue;
            if (entity->iHealth < 1)
                continue;
            Vec2 Foot2d, Head2d;
            if (WorldToScreen(entity->vecOrigin, Foot2d))
            {
                Vec3 Head3d = GetBonePos(entity, 8);
                Head3d.z += 8;
                DrawLine(Foot2d.x, Foot2d.y, eEngine.width / 2, eEngine.height, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                if (WorldToScreen(Head3d, Head2d))
                {
                    DrawEspBox2D(Foot2d, Head2d, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                    DrawHealthArmor(Foot2d, Head2d, 4, entity);
                }
            }
        }
    }
    // call original EndScene
    return oEndScene(gDevice);
}

DWORD WINAPI internalMain(HMODULE hMod) {
#ifdef _DEBUG
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
#endif 

    // !!! This will get a vtable with functions adresses, but not the vtable of the d3d object in process !!!
    void** vTable = GetD3D9Device();
    // now we have a vTable with functions addresses(or not)
    if (vTable)
    {
        detour* EndSceneDetour = new detour((char*)vTable[42], (char*)hkEndScene, 7);
        oEndScene = (_EndScene)EndSceneDetour->trampHook();
        // VK_PRIOR = Page Up
        // VK_END = End
        // VK_NEXT = Page dn
        // VK_HOME = home
        // 0x30 = 0 (key)
        HANDLE aimthr = 0, aimthrfov = 0;
        while (!GetAsyncKeyState(VK_END))
        {
            if (aimbot || aimbotF || esp) eClient.updateVM();
            if (!eEngine.height && gDevice)
                GetViewportSize();
            if (GetAsyncKeyState(VK_PRIOR) & 1)
            {
                eClient.aimSmooth < 100000 ? eClient.aimSmooth += 5000 : 0;
                printf("[+] Smooth increased is now %d\n", eClient.aimSmooth);
            }
            if (GetAsyncKeyState(VK_NEXT) & 1)
            {
                eClient.aimSmooth > 5000 ? eClient.aimSmooth -= 5000 : 0;
                printf("[-] Smooth decreased is now %d\n", eClient.aimSmooth);
            }
            /* FOR NO RECOIL CONTROL SMOOTH VALUE
            if (GetAsyncKeyState(VK_MENU))
            {
                if (GetAsyncKeyState(VK_PRIOR))
                    eClient.recoilSmooth < 30000 ? eClient.recoilSmooth += 1000 : 0;
                if (GetAsyncKeyState(VK_NEXT))
                    eClient.recoilSmooth > 1000 ? eClient.recoilSmooth -= 1000 : 0;
            }
            */
            if (GetAsyncKeyState(0x30) & 1)
            {
                esp = !esp;
                if (esp)
                    printf("[O] ESP Activated\n");
                else
                    printf("[X] ESP Deactivated\n");
            }
            if (GetAsyncKeyState(0x39) & 1)
            {
                aimbot = !aimbot;
                if (aimbot == true)
                {
                    printf("[O] Simple Aimbot activated (aimfov=?)\n");
                    aimthr = CreateThread(NULL, 0, aimbotTH, &eClient.aimSmooth, 0, NULL);
                }
                else if (aimthr)
                {
                    printf("[X] Simple Aimbot deactivated\n");
                    TerminateThread(aimthr, NULL);
                }
            }
            if (GetAsyncKeyState(0x38) & 1)
            {
                aimbotF = !aimbotF;
                if (aimbotF == true)
                {
                    printf("[O] FOV Aimbot activated (aimfov=?)\n");
                    aimthrfov = CreateThread(NULL, 0, aimbotFOV, &eClient.aimSmooth, 0, NULL);
                }
                else if (aimthrfov)
                {
                    printf("[X] FOV Aimbot deactivated\n");
                    TerminateThread(aimthrfov, NULL);
                }
            }
            Sleep(2);
        }
        if (oEndScene != nullptr)
        {
            EndSceneDetour->unhook();
        }
        if (aimthr)
            TerminateThread(aimthr, NULL);
        if (aimthrfov)
            TerminateThread(aimthrfov, NULL);

        delete EndSceneDetour;
    }
#ifdef _DEBUG
    if (f != nullptr) fclose(f);
    FreeConsole();
#endif
    FreeLibraryAndExitThread(hMod, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Create new thread
        HANDLE tHndl = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)internalMain, hModule, 0, 0);
        if (tHndl) CloseHandle(tHndl);
        else return FALSE;
        break;
    }
    return TRUE;
}

