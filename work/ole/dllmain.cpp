#include "stdafx.h"
#include "dllmain.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

_declspec(dllexport) RawFileInterface * _stdcall GetRawFileInterface(void)
{
	try
	{
		//return (RawFileInterface *)new OleFile();
	}
	catch (std::bad_alloc)
	{
		return NULL;
	}
}
