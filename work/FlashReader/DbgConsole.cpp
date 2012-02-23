#include "stdafx.h"
#ifdef SDEBUG
#include <stdio.h>
#include <windows.h>
#include "DbgConsole.h"


SDbgConsole SDBG(DEBUG_FILE);

SDbgConsole::SDbgConsole()
{
	_InitConsole();
	m_hFile = INVALID_HANDLE_VALUE;
}

SDbgConsole::SDbgConsole(const wchar_t * FileName)
{
	_InitConsole();
	m_hFile = CreateFile(
		FileName,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		0,0);
	{
		BYTE buf[] = {0xFF, 0xFE};
		DWORD rw = 0;
		WriteFile(m_hFile,buf,2,&rw,NULL);
	}
}

void SDbgConsole::_InitConsole()
{
	if(DEBUG_CONSOLE)
	{
		AllocConsole();
		m_hConsole = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL);
		SetConsoleActiveScreenBuffer(m_hConsole);
		SetConsoleTitle(L"SDebug Window v.1.0 Copyright Sergey Samchuk, 2009");
	}
	else m_hConsole = INVALID_HANDLE_VALUE;
	m_hMutex = CreateMutex(NULL,FALSE,NULL);
}

SDbgConsole::~SDbgConsole()
{
	_Print(L"************************************************\r\nEnd SDebug Log.\r\n",tcWhite);
	if(m_hConsole != INVALID_HANDLE_VALUE) 
	{
		Sleep(DEBUG_WAIT);
		CloseHandle(m_hConsole);
	}
	if(m_hMutex != INVALID_HANDLE_VALUE) CloseHandle(m_hMutex);
	if(m_hFile != INVALID_HANDLE_VALUE) CloseHandle(m_hFile);
	FreeConsole();
}

void SDbgConsole::_Print(const wchar_t* msg, TextColor color)
{
	if(WaitForSingleObject(m_hMutex,INFINITE) == WAIT_OBJECT_0)
	{
		DWORD rw = 0;
		if(m_hConsole != INVALID_HANDLE_VALUE) 
		{
			SetConsoleTextAttribute(m_hConsole,GetTextAttr(color));
			WriteConsole(m_hConsole,msg,wcslen(msg),&rw,NULL);
		}
		if(m_hFile != INVALID_HANDLE_VALUE) 
		{
			WriteFile(m_hFile,msg,wcslen(msg)*sizeof(wchar_t),&rw,NULL);
			FlushFileBuffers(m_hFile);
		}
		ReleaseMutex(m_hMutex);
	}
}

WORD SDbgConsole::GetTextAttr(TextColor color)
{
	switch(color)
	{
	case tcRed: return FOREGROUND_RED | FOREGROUND_INTENSITY;
	case tcGreen: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	case tcBlue: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	case tcWhite: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	default:
	case tcGrey:  return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}
}

void SDbgConsole::Print(TextColor color, const wchar_t * Format, ...)
{
    va_list _Arglist;
    _crt_va_start(_Arglist, Format);
	int len = _vscwprintf(Format,_Arglist)+1;
	wchar_t* buf = reinterpret_cast<wchar_t*>(malloc(len*sizeof(wchar_t)));
    vswprintf_s(buf, len, Format, _Arglist);
    _crt_va_end(_Arglist);
	_Print(buf,color);
	free(buf);
}

void SDbgConsole::Print(const wchar_t * Format, ...)
{
    va_list _Arglist;
    _crt_va_start(_Arglist, Format);
	int len = _vscwprintf(Format,_Arglist)+1;
	wchar_t* buf = reinterpret_cast<wchar_t*>(malloc(len*sizeof(wchar_t)));
    vswprintf_s(buf, len, Format, _Arglist);
    _crt_va_end(_Arglist);
	_Print(buf);
	free(buf);
}

#endif
