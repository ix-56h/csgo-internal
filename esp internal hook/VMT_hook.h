#pragma once
#include <windows.h>

class VMT_hook
{
	public:
		VMT_hook(void **vTable);
		~VMT_hook();
		void* GetByAddr(unsigned short index);
		void* hook(unsigned short index, void *OurFunc);
		bool unhook(unsigned short index);
		bool unhookAll();

	private:
		void** m_vTable;
		unsigned short m_nbFuncs;
		void** m_originFuncs = nullptr;
		unsigned short GetNbrFuncs();
};
