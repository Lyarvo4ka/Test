#pragma once

#ifndef _W32APP
#define _W32APP

#include "Global\Global.h"
#include <commctrl.h>

namespace W32Lib
{
	#define WND_CLASS_NAME		_T("WndWindow")

	using namespace Gdiplus;

	class W32MessageLoop
	{
	protected:
		HWND hwnd;
		UINT msg_filter_min;
		UINT msg_filter_max;
	public:
		W32MessageLoop(HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
		~W32MessageLoop(void) {};

		HWND GetHwnd(void) {return hwnd;}
		UINT GetMsgFilterMin(void) {return msg_filter_min;}
		UINT GetMsgFilterMax(void) {return msg_filter_max;}

		HWND SetHwnd(void) {return hwnd;}
		void SetMsgFilterMin(UINT new_val) {msg_filter_min = new_val;}
		void SetMsgFilterMax(UINT new_val) {msg_filter_max = new_val;}

		DWORD Run(void); 
	};

	class W32App : public W32MessageLoop
	{
	private:
		ULONG_PTR gdiplus_token;
		GdiplusStartupInput gdiplus_startup_input;
	public:
		W32App(void);
		~W32App(void);
		void InitInstance(void);
	};
}

#endif

