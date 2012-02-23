#ifndef _BUTTON
#define _BUTTON

#include "main.h"

using namespace W32Lib;

class PButton : public Wnd
{
public:
	PButton() {}
	~PButton() {}

	void Create(LPCTSTR button_text, int x, int y, int nWidth, int nHeight, Wnd *parent_wnd, DWORD id);
	BOOL GetIdealSize(SIZE *sz) {return (BOOL)SendMessage(GetHandle(), BCM_GETIDEALSIZE, NULL, (LPARAM)sz);}
	//void SetDontClick(BOOL state) {::SendMessage(GetHandle(), BM_SETDONTCLICK, (WPARAM)state, 0);}
};

class GroupBox : public Wnd
{
public:
	GroupBox() {}
	~GroupBox() {}

	BOOL Create(LPCTSTR text, int x, int y, int nWidth, int nHeight, Wnd *parent_wnd, DWORD id);
};

#endif //_BUTTON
