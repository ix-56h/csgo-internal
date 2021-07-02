#include "VMT_hook.h"
/*
	class : VMT_hook
	Function : Constructor
	Purpose : Get vTable based on the vTable passed by argument, create a copy
*/
VMT_hook::VMT_hook(void** vTable) : m_vTable(vTable)
{
	m_nbFuncs = GetNbrFuncs();
	m_originFuncs = new void*[m_nbFuncs];
	for (unsigned short i = 0; i < m_nbFuncs; i++)
	{
		m_originFuncs[i] = GetByAddr(i);
	}
}

/*
	class : VMT_hook
	Function : Destructor
	Purpose : 
*/
VMT_hook::~VMT_hook()
{
	unhookAll();
	delete[] m_originFuncs;
}

/*
	class : VMT_hook
	Function : GetByAddr
	Purpose : Get function in vtable by index and return function address
*/
void* VMT_hook::GetByAddr(unsigned short index)
{
	return (index > m_nbFuncs ? nullptr : m_vTable[index]);
}

/*
	class : VMT_hook
	Function : GetNbrfuncs
	Purpose : Return the number of functions in vtable by guessing
*/
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

/*
	class : VMT_hook
	Function : hook
	Purpose : Get the VMT index function to hook and a pointer to our function and replace the vTable entry by our address
*/
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

/*
	class : VMT_hook
	Function : unhook
	Purpose : Restore the vTable entry pointed by dereferencing index and set the original function address
*/
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

/*
	class : VMT_hook
	Function : unhookAll
	Purpose : Restore all vTable entries with original functions addresses
*/
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