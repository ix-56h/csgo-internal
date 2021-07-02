// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "internal.h"
#include "D3Device.h"
#include "VMT_hook.h"
#include <windows.h>
#include <iostream>

// typedef the function prototype of EndScene
//typedef HRESULT APIENTRY EndScene(LPDIRECT3DDEVICE9);
// Declare a pointer on this function prototype
//EndScene *oEndScene = NULL;

using _EndScene = HRESULT(APIENTRY)(LPDIRECT3DDEVICE9);
_EndScene *oEndScene = NULL;

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName) {
    return (uintptr_t)GetModuleHandleW(moduleName);
}

//void hkFunc(void) { void }

HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice)
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

    //get vtable pointer with GetD3D9Device();
    void** vTable = GetD3D9Device();
    // now we have addr of vtable (or not)
    //void* d3ddevice[119];
    //if (GetD3D9Device(d3ddevice, sizeof(d3ddevice)))
    if (vTable)
    {
        //void** vTable = *reinterpret_cast<void***>(addr);
        printf("vTable = %p\nEndScene = %p\nourEndScene = %p\n", vTable, vTable[42], (void*)hkEndScene);
        //printf("%p\n%p\n", d3ddevice, d3ddevice[42]);
        // init hooking
        VMT_hook* vmt = new VMT_hook(vTable);
        if (esp == true)
        {
            printf("oEndScene = %p\n", oEndScene);
            oEndScene = (_EndScene*)vmt->hook(42, (void*)hkEndScene);
            printf("oEndScene = %p\n", oEndScene);
            printf("new vTable[42] = %p\n", vTable[42]);
        }
        while (!GetAsyncKeyState(VK_ESCAPE))
        {
            //continue;
            //printf("vTable[42] = %p\n", vTable[42]);
            if (GetAsyncKeyState(VK_DELETE))
            {
                esp = !esp;
                if (esp == false)
                    vmt->unhook(42);
            }
        }
        delete vmt;
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

