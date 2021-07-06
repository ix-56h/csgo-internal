#include "D3Device.h"
#include "ClientEngine.h"

static HWND window;
extern LPDIRECT3DDEVICE9 gDevice;
extern IEngine eEngine;

/*
	Function : GetViewportSize()
	Purpose : get the width and the height of the current device
*/
void GetViewportSize()
{
	D3DVIEWPORT9 Viewport;
	gDevice->GetViewport(&Viewport);
	eEngine.width = Viewport.Width;
	eEngine.height = Viewport.Height;
}

/*
	Function : EnumWindowsCallB()
	Purpose : Callback for EnumWindows, he return false and break EnumWindows, before he set static HWND window to the good handle
*/
BOOL CALLBACK EnumWindowsCallB(HWND handle, LPARAM lParam)
{
	DWORD curProcId;
	GetWindowThreadProcessId(handle, &curProcId);
	if (GetCurrentProcessId() != curProcId)
		return true;
	window = handle;
	return false;
}

/*
	Function : GetWindow()
	Purpose : Return the window handle of the current process
*/
HWND GetWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallB, NULL);

	return window;
}

/*
	Function : GetD3D9Device()
	Purpose : Return a void**vTable containing functions addresses of the object D3DBase (Begin/EndScene...)
			  Create a dummy device and delete him after copying the vTable.
		!!! Return a copy of the dummyDevice vTable, containing all functions addresses who's loaded in memory !!!
*/
//bool GetD3D9Device(void** pTable, size_t size)
void **GetD3D9Device()
{
	// create IDirect3d9interface and get interface returned
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return nullptr;

	IDirect3DDevice9 *pDummyDevice = NULL;

	// Setting up presentation parameters
	D3DPRESENT_PARAMETERS d3dpresent = {};
	// set windoed to false
	d3dpresent.Windowed = false;
	// set hDeviceWindow
	d3dpresent.hDeviceWindow = GetWindow();
	// idk ? something about swap buffers
	d3dpresent.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT dummyDevice = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpresent.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpresent, &pDummyDevice);
	if (dummyDevice != D3D_OK)
	{
		// setup params with windowed
		d3dpresent.Windowed = !d3dpresent.Windowed;
		dummyDevice = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpresent.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpresent, &pDummyDevice);

		if (dummyDevice != D3D_OK)
		{
			pD3D->Release();
			return nullptr;
		}
	}
	// Copy the vTable into pTable
	void** ret = *reinterpret_cast<void***>(pDummyDevice);
	//Clean memory
	pD3D->Release();
	pDummyDevice->Release();
	return ret;
}