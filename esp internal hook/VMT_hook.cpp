#include "VMT_hook.h"
//	1 : init hook with vtable
//		1.1: save nmbr funcs
//		1.2: save copy off addresses in vtables
// 
//	2 : use VMT_hook::hook for hook the actual function
//		2.1: virtual protect and other stuff
//	3 : use VMT_hook::unhook for restor initial addresses for [index]'th function
//	4 : VMT_hook::unhookAll for restre all initial addresses
//		4.1 : for each the vtable and reset all addresses with the savec copy respective index
// ~VMT_hook destructor call unhookall() and use delete saved vtable copy (free)

VMT_hook::VMT_hook(void** vTable) : m_vTable(vTable)
{
	//m_nbFuncs = GetNbrFuncs();
	m_nbFuncs = GetNbrFuncs();
	m_originFuncs = new void*[m_nbFuncs];
	for (unsigned short i = 0; i < m_nbFuncs; i++)
	{
		m_originFuncs[i] = GetByAddr(i);
	}
}

VMT_hook::~VMT_hook()
{
	unhookAll();
	delete[] m_originFuncs;
}

void* VMT_hook::GetByAddr(unsigned short index)
{
	return (index > m_nbFuncs ? nullptr : m_vTable[index]);
}

unsigned short VMT_hook::GetNbrFuncs()
{
	unsigned short index = 0; //index
	MEMORY_BASIC_INFORMATION mbiBuffer = { 0 }; //buffer to store the information of some memory region
	while (true)
	{
		if (!m_vTable[index]) //if vtable[index] is null
		{
			break; //exit loop
		}
		if (!VirtualQuery(m_vTable[index], &mbiBuffer, sizeof(mbiBuffer))) //if VirtualQuery failed
		{
			break; //exit loop
		}
#define CAN_EXECUTE (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)
		if ((mbiBuffer.State != MEM_COMMIT) || (mbiBuffer.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(mbiBuffer.Protect & CAN_EXECUTE)) //if the region isn't commited and/or executable
		{
			break; //exit loop
		}
		++index;
	}
	return index;
}

void* VMT_hook::hook(unsigned short index, void* OurFunc)
{
	DWORD oldProtect = NULL;
	void* oldFunc = nullptr;

	if (index < m_nbFuncs && OurFunc)
	{
		VirtualProtect(m_vTable + index, 0x4, PAGE_EXECUTE_READWRITE, &oldProtect);
		oldFunc = m_vTable[index];
		m_vTable[index] = OurFunc;
		VirtualProtect(m_vTable + index, 0x4, oldProtect, &oldProtect);
		return oldFunc;
	}
	return nullptr;
}

bool VMT_hook::unhook(unsigned short index)
{
	DWORD oldProtect = NULL;
	if (index < m_nbFuncs)
	{
		VirtualProtect(m_vTable + index, 0x4, PAGE_EXECUTE_READWRITE, &oldProtect);
		m_vTable[index] = m_originFuncs[index];
		VirtualProtect(m_vTable + index, 0x4, oldProtect, &oldProtect);
		return true;
	}
	return false;
}

bool VMT_hook::unhookAll()
{
	bool ret = true;
	for (unsigned short i = 0; i < m_nbFuncs; i++)
	{
		if (m_vTable[i] == m_originFuncs[i])
			continue;
		ret = (!unhook(i) && ret ? false : true);
	}
	return ret;
}