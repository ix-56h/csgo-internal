// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "internal.h"
#include <windows.h>
#include <iostream>
#include <d3dx9.h>

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName) {
    return (uintptr_t)GetModuleHandleW(moduleName);
}

DWORD WINAPI internalMain(HMODULE hMod) {
#ifdef _DEBUG
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
#endif 

    bool esp = false;

    int i = 0, x = 0;

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        if (GetAsyncKeyState(VK_DELETE))
        {
            esp = !esp;
        }
        if (esp == true)
        {

        }
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

