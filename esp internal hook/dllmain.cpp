// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "internal.h"
#include "D3Device.h"
#include "VMT_hook.h"
#include "detour.h"
#include <windows.h>
#include <iostream>

IClient eClient;
extern size_t windowHeight, windowWidth;
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
    for (size_t i = 0; i < eClient.maxPlayers; i++)
    {
        pEntity* entity = *reinterpret_cast<pEntity**>(eClient.pEntityList + (i * 0x10));
        if (!entity)
            continue;
        if ((uintptr_t)entity == (uintptr_t)(eClient.localPlayer))
            continue;
        if (eClient.localPlayer->iHealth < 1 || entity->iHealth < 1)
            continue;
        if (eClient.localPlayer->iTeamNum == entity->iTeamNum)
            continue;
        if (!entity->bDormant)
        {
            Vec2 Foot2d, Head2d;
            if (WorldToScreen(entity->vecOrigin, Foot2d))
            {
                Vec3 Head3d = GetBonePos(entity, 8);
                Head3d.z += 8;
                DrawLine(Foot2d.x, Foot2d.y, windowWidth / 2, windowHeight, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                if (WorldToScreen(Head3d, Head2d))
                {
                    DrawEspBox2D(Foot2d, Head2d, 2, D3DCOLOR_ARGB(255, 255, 0, 0));

                    int height = ABS(Foot2d.y - Head2d.y);
                    int dX = (Foot2d.x - Head2d.x);

                    float healthPerc = entity->iHealth / 100.f;
                    float armorPerc = entity->armorValue / 100.f;

                    Vec2 botHealth, topHealth, botArmor, topArmor;
                    int healthHeight = height * healthPerc;
                    int armorHeight = height * armorPerc;

                    botHealth.y = botArmor.y = Foot2d.y;

                    botHealth.x = Foot2d.x - (height / 4) - 2;
                    botArmor.x = Foot2d.x + (height / 4) + 2;

                    topHealth.y = Head2d.y + height - healthHeight;
                    topArmor.y = Head2d.y + height - armorHeight;

                    topHealth.x = Foot2d.x - (height / 4) - 2 - (dX * healthPerc);
                    topArmor.x = Foot2d.x + (height / 4) + 2 - (dX * armorPerc);

                    DrawLineVec(botHealth, topHealth, 5, D3DCOLOR_ARGB(255, 46, 139, 87));
                    DrawLineVec(botArmor, topArmor, 5, D3DCOLOR_ARGB(255, 30, 144, 255));
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
    bool esp = true;

    // !!! This will get a vtable with functions adresses, but not the vtable of the d3d object in process !!!
    void** vTable = GetD3D9Device();
    // now we have a vTable with functions addresses(or not)
    if (vTable)
    {
        // getting viewport resolution
        
        detour *EndSceneDetour = new detour((char*)vTable[42], (char*)hkEndScene, 7);
        //void** vTable = *reinterprest_cast<void***>(addr);
        //printf("EndScene = %p\nourEndScene = %p\n", vTable[42], (void*)hkEndScene);
        //printf("viewMatrix = %" PRIxPTR "\n", (uintptr_t)(eClient.pClientBase + hazedumper::signatures::dwViewMatrix));
        //EndSceneDetour->hook();
        oEndScene = (_EndScene)EndSceneDetour->trampHook();
        // can't use VMT_hook, so need to use detour

        while (!GetAsyncKeyState(VK_ESCAPE))
        {
            if (!windowHeight && gDevice)
                GetViewportSize();
            eClient.updateVM();
            if (GetAsyncKeyState(VK_DELETE))
            {
                esp = !esp;
                if (esp == false)
                    EndSceneDetour->unhook();
            }
        }
        if (oEndScene != nullptr)
            EndSceneDetour->unhook();
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

