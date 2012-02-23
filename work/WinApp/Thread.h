#pragma once

#include "W32Lib.h"

class Thread
{
private:
	HANDLE hThread;
	LPVOID thread_parameter;
	LPTHREAD_START_ROUTINE thread_routine;

	static DWORD WINAPI ThreadStartRoutine(LPVOID parameter)
	{
		int x = 0;
		return ((Thread *)parameter)->thread_routine(((Thread *)parameter)->thread_parameter);
	}

public:
	Thread(void) : hThread(NULL), thread_parameter(NULL), thread_routine(NULL) {}
	~Thread(void) {if (hThread) ::CloseHandle(hThread);}

	BOOL Create(LPTHREAD_START_ROUTINE procedure, LPVOID parameter)
	{
		if ( procedure && (hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread::ThreadStartRoutine, this, 0, NULL)) )
		{
			thread_parameter = parameter;
			thread_routine = procedure;
			return TRUE;
		}
		else
			return FALSE;
	}
};
