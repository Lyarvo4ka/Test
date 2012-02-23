#include "stdafx.h"
#include <stdio.h>
#include "HideMsg.h"
#include <shellapi.h>


unsigned long files_counter = 0;
unsigned long bad_files_counter = 0;
unsigned long ok_files_counter = 0;


BOOL TestPdf(WIN32_FIND_DATA &fData, LPTSTR path);
HWND FindWnd(HWND hParent, LPCTSTR lpWindowName, DWORD milliseconds);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
//{
//	DWORD err;
//
//	HMODULE hModule = ::LoadLibrary(_T("KeyHook.dll"));
//	if (hModule == NULL)
//	{
//		err = GetLastError();
//		return 0;
//	}
//
//	HOOKPROC hookProc = (HOOKPROC)::GetProcAddress(hModule, "KbdHook");
//	if (hookProc == NULL)
//		return 0;
//
//	HHOOK hHook = ::SetWindowsHookEx(WH_KEYBOARD, hookProc, hModule, NULL);
//	if (!hHook)
//	{
//		err = GetLastError();
//		return 0;
//	}
//
//	while (!bQuit) {;}  
//
//	return 
//}




{
	HWND hFindWnd;
	HANDLE hEvent;

	if (NULL == (hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL)))
		return 0;
	

	for (;;)
	{
		if (NULL != (hFindWnd = ::FindWindow(NULL, _T("File already exists"))))
		{
			//if (NULL != (hFindWnd = ::FindWindowEx(hFindWnd, NULL, NULL, _T("&Overwrite"))))
			//{
				LRESULT  res;
				res = ::PostMessage(hFindWnd, WM_QUIT, 0x00, 0x00);
				int x = 0;
			//}
		}
		::WaitForSingleObject(hEvent, 200);
	}
	return 0;
}


//{
//	HANDLE hFind;
//	WIN32_FIND_DATA findData;
//	DWORD err;
//
//	char findPath[1024] = {0};
//	sprintf_s(findPath, 1024, _T("%s\\*.pdf"), lpCmdLine);
//
//	hFind = ::FindFirstFile(findPath, &findData);
//	if (hFind != INVALID_HANDLE_VALUE)
//	{
//		++files_counter;
//		if(TestPdf(findData, lpCmdLine))
//			++ok_files_counter;
//		else
//			++bad_files_counter;
//
//		memset(&findData, 0x00, sizeof(findData));
//		while(FindNextFile(hFind, &findData))
//		{
//			++files_counter;
//			if(TestPdf(findData, lpCmdLine))
//				++ok_files_counter;
//			else
//				++bad_files_counter;
//
//			memset(&findData, 0x00, sizeof(findData));
//		}
//	}
//	else
//		err = ::GetLastError();
//
//	char msg[2048] = {0};
//	sprintf_s(msg, 2048, "Проверка файлов звершена.\n" \
//						 " - проверено файлов: %d\n"\
//						 " - целых файлов: %d\n"\
//						 " - повреждённых файлов: %d\n", files_counter, ok_files_counter, bad_files_counter);
//
//	::MessageBox(NULL, msg, "Pdf check", MB_OK | MB_TOPMOST | MB_ICONINFORMATION);
//
//	return 0;
//}

void MarkAsBad(LPTSTR fileName)
{
	TCHAR newName[1024] = {0};
	sprintf_s(newName, 1024, "%s.bad", fileName);
	if (!MoveFile(fileName, newName))
	{
		DWORD err = ::GetLastError();
		int x = 0;
	}
}

BOOL TestPdf(WIN32_FIND_DATA &fData, LPTSTR path)
{
	HWND hWnd;
	char filePath[1024] = {0};
	char wndTitle[1024] = {0};

	sprintf_s(filePath, 1024, "%s\\%s", path, fData.cFileName);

	ShellExecute(NULL, "open", filePath, NULL, NULL, SW_SHOWNA);

	sprintf_s(wndTitle, 1024, "%s - Adobe Reader", fData.cFileName);

	for (int i = 0; i < 5; ++i)
	{
		if (hWnd = FindWnd(NULL, wndTitle, 200))
		{
			HWND hBeyondWnd;
			if (hBeyondWnd = FindWnd(NULL, "Beyond Adobe Reader", 0))
				::SendMessage(hBeyondWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

			::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			return TRUE;
		}

		if (hWnd = FindWnd(NULL, "Password", 0))
		{
			HWND hGroupBox;
			if (hGroupBox = FindWnd(hWnd, "", 0))
			{
				HWND hCancelButton;
				if (hCancelButton = FindWnd(hGroupBox, "Cancel", 0))
					::SendMessage(hCancelButton, WM_USER + 40, 0, (LPARAM)hCancelButton);

				HWND hBeyondWnd;
				if (hBeyondWnd = FindWnd(NULL, "Beyond Adobe Reader", 0))
					::SendMessage(hBeyondWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

				if (hWnd = FindWnd(NULL, "Adobe Reader", 0))
					::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

				return TRUE;
			}
		}

		if (hWnd = FindWnd(NULL, "Adobe Reader", 0))
		{
			HWND hGroupBox;
			if (hGroupBox = FindWnd(hWnd, "", 0))
			{
				HWND hOkButton;
				if (hOkButton = FindWnd(hGroupBox, "OK", 0))
					::SendMessage(hOkButton, WM_USER + 40, 0, (LPARAM)hOkButton);

				HWND hBeyondWnd;
				if (hBeyondWnd = FindWnd(NULL, "Beyond Adobe Reader", 0))
					::SendMessage(hBeyondWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

				if (hWnd = FindWnd(NULL, "Adobe Reader", 0))
					::SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

				MarkAsBad(filePath);
				return FALSE;
			}
		}
	}
	MarkAsBad(filePath);
	return FALSE;
}

HWND FindWnd(HWND hParent, LPCTSTR lpWindowName, DWORD milliseconds)
{
	HWND hFindWnd;
	HANDLE hEvent;

	if (NULL == (hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL)))
		return NULL;

	for (DWORD i = 0; i < 10; ++i)
	{
		if (hFindWnd = ::FindWindowEx(hParent, NULL, NULL, lpWindowName))
			return hFindWnd;
		::WaitForSingleObject(hEvent, milliseconds/10);
	}

	return NULL;
}