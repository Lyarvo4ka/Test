// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <tchar.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
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

_declspec(dllexport) LRESULT CALLBACK KbdHook(int code, WPARAM wParam, LPARAM lParam)
{

	if (lParam >= KF_UP)
	{
		UINT ch = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
		TCHAR buffer[255] = {0};

		_stprintf_s(buffer, _T("Pressed %c"), ch);
		::MessageBox(NULL, buffer, _T("Keyboard Hook"), MB_OK);
	}

	return 0;
}

