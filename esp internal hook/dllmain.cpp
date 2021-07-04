// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "internal.h"
#include "D3Device.h"
#include "VMT_hook.h"
#include "detour.h"
#include <windows.h>
#include <iostream>

// typedef the function prototype of EndScene
//typedef HRESULT APIENTRY EndScene(LPDIRECT3DDEVICE9);
// Declare a pointer on this function prototype
//EndScene *oEndScene = NULL;

typedef HRESULT(__stdcall* _EndScene)(LPDIRECT3DDEVICE9 pDevice);
_EndScene oEndScene = nullptr;

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName) {
    return (uintptr_t)GetModuleHandleW(moduleName);
}

//void hkFunc(void) { void }

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
    printf("Hooked\n");
    // call original EndScene
    return oEndScene(pDevice);
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
        detour *EndSceneDetour = new detour((char*)vTable[42], (char*)hkEndScene, 7);
        //void** vTable = *reinterprest_cast<void***>(addr);
        printf("EndScene = %p\nourEndScene = %p\n", vTable[42], (void*)hkEndScene);
        //EndSceneDetour->hook();
        oEndScene = (_EndScene)EndSceneDetour->trampHook();
        // can't use VMT_hook, so need to use detour
        if (esp == true)
        {
            printf("oEndScene = %p\n", oEndScene);
        }
        while (!GetAsyncKeyState(VK_ESCAPE))
        {
            //continue;
            //printf("vTable[42] = %p\n", vTable[42]);
            if (GetAsyncKeyState(VK_DELETE))
            {
                esp = !esp;
                //if (esp == false)
                    //vmt->unhook(42);
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

