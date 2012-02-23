#include "PassHack.h"

#define MAX_WND_TEXT						256
#define MAX_PASS_LEN						15
#define MAIN_WINDOW_NAME					_T("StoreJet elite - Подключиться")
#define ERROR_WINDOW_NAME					_T("Ошибка")
#define OK_BUTTON_NAME						_T("Login")
#define CANCEL_BUTTON_NAME					_T("Cancel")


TCHAR *charset = _T(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_!-+=@#$%^&*()");

Password::Password(TCHAR *char_set, DWORD len) : charset(char_set), pwd_len(len)
{
	charset_len = _tcslen(charset);
	max_index = charset_len - 1;
	pwd_byte = new BYTE[pwd_len];
	pwd_str = new TCHAR[pwd_len + 1];
	memset(pwd_str, 0x00, sizeof(TCHAR)+pwd_len);
}

Password::~Password(void)
{
	delete pwd_byte;
	delete pwd_str;
}

BOOL Password::Roll(void)
{
	BOOL b_overflow = FALSE;
	for (DWORD i = 0; i < pwd_len; i++)
	{
		if (pwd_byte[i] < max_index)
		{
			pwd_byte[i]++;
			b_overflow = FALSE;
		}
		else
		{
			pwd_byte[i] = 0x00;
			b_overflow = TRUE;
		}
	}
	return !b_overflow;
}

const TCHAR *Password::GetPasswordString(void)
{
	for (DWORD i = 0; i < pwd_len; i++)
	{
		pwd_str[i] = charset[pwd_byte[i]];
	}
	return pwd_str;
}

BOOL CALLBACK GetLoginButton(HWND hwnd, LPARAM lParam)
{
	TCHAR class_name[MAX_WND_TEXT] = {0};
	TCHAR wnd_text[MAX_WND_TEXT] = {0};
	::RealGetWindowClass(hwnd, class_name, MAX_WND_TEXT);
	if (_tcscmp(class_name, _T("Button")) == 0x00)
	{
		::GetWindowText(hwnd, wnd_text, MAX_WND_TEXT);
		if (_tcscmp(wnd_text, _T("Login")) == 0x00)
		{
			*(HWND *)lParam = hwnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CALLBACK GetCancelButton(HWND hwnd, LPARAM lParam)
{
	TCHAR class_name[MAX_WND_TEXT] = {0};
	TCHAR wnd_text[MAX_WND_TEXT] = {0};
	::RealGetWindowClass(hwnd, class_name, MAX_WND_TEXT);
	if (_tcscmp(class_name, _T("Button")) == 0x00)
	{
		::GetWindowText(hwnd, wnd_text, MAX_WND_TEXT);
		if (_tcscmp(wnd_text, _T("Cancel")) == 0x00)
		{
			*(HWND *)lParam = hwnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	TCHAR class_name[MAX_WND_TEXT] = {0};
	::RealGetWindowClass(hwnd, class_name, MAX_WND_TEXT);
	if (_tcscmp(class_name, _T("Edit")) == 0x00)
	{
		*(HWND *)lParam = hwnd;
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR wnd_text[MAX_WND_TEXT] = {0};
	::GetWindowText(hwnd, wnd_text, MAX_WND_TEXT);
	if (_tcscmp(wnd_text, MAIN_WINDOW_NAME) == 0x00)
	{
		*(HWND *)lParam = hwnd;
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK GetErrorWindow(HWND hwnd, LPARAM lParam)
{
	TCHAR wnd_text[MAX_WND_TEXT] = {0};
	::GetWindowText(hwnd, wnd_text, MAX_WND_TEXT);
	if (_tcscmp(wnd_text, ERROR_WINDOW_NAME) == 0x00)
	{
		*(HWND *)lParam = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND GetStoreJetEdit(HWND main_wnd)
{
	HWND edit_wnd = NULL;
	::EnumChildWindows(main_wnd, EnumChildProc, (LPARAM )&edit_wnd);
	return edit_wnd;
}

HWND GetStoreJetMainWindow(void)
{
	HWND main_wnd = NULL;
	::EnumWindows(EnumWindowsProc, (LPARAM )&main_wnd);
	return main_wnd;
}

HWND GetStoreJetLoginButton(HWND main_wnd)
{
	HWND login_btn = NULL;
	::EnumChildWindows(main_wnd, GetLoginButton, (LPARAM)&login_btn);
	return login_btn;
}

HWND GetStoreJetCancelButton(HWND main_wnd)
{
	HWND cancel_btn = NULL;
	::EnumChildWindows(main_wnd, GetCancelButton, (LPARAM)&cancel_btn);
	return cancel_btn;
}

HWND GetStoreJetErrorWindow(void)
{
	HWND error_wnd = NULL;
	for (int i = 0; i < 10; i++)
	{
		EnumWindows(GetErrorWindow, (LPARAM)&error_wnd);
		if (error_wnd)
			break; 
	}
	return error_wnd;
}

void CloseStoreJetErrorWindow(HWND error_wnd)
{
	::SendMessage(error_wnd, WM_CLOSE, 0, 0);
}

int PassHackMain()
{
	HWND main_wnd = NULL;
	HWND error_wnd = NULL;
	HWND edit_wnd = NULL;
	HWND login_btn = NULL;
	HWND cancel_btn = NULL;

	main_wnd = GetStoreJetMainWindow();
	if (main_wnd)
	{
		_tprintf(_T("StoreJet main window find\n"));
		edit_wnd = GetStoreJetEdit(main_wnd);
		if (edit_wnd)
		{
			_tprintf(_T("Edit control find\n"));
			login_btn = GetStoreJetLoginButton(main_wnd);
			if (login_btn)
			{
				_tprintf(_T("Login button find\n"));
				cancel_btn = GetStoreJetCancelButton(main_wnd);
				if (cancel_btn)
				{
					_tprintf(_T("Cancel button find\n"));

					// =================================================
					
					_tprintf(_T("Start BrootForce\n"));

					for(int pwd_len = 1; pwd_len < MAX_PASS_LEN; pwd_len++)
					{
						Password pwd(charset, pwd_len);
						while (pwd.Roll())
						{
							_tprintf(_T("%s\n"), pwd.GetPasswordString());
							_getch();
						}
					}

					int x = 0;



					// =================================================
				}
			}
		}
	}
	
	//::SendMessage(edit_wnd, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)_T("Test"));
	//::PostMessage(login_btn, BM_CLICK, 0, 0);

	//error_wnd = GetStoreJetErrorWindow();
	//CloseStoreJetErrorWindow(error_wnd);

	_tprintf(_T("\nDone. Press any key ..."));
	_getch();

	return 0;
}