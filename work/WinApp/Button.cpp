#include "Button.h"

void PButton::Create(LPCTSTR button_text, int x, int y, int nWidth, int nHeight, Wnd *parent_wnd, DWORD id)
{
	Wnd::CreateEx(NULL, _T("BUTTON"), button_text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
				x, y, nWidth, nHeight, parent_wnd, (HMENU)id, ::GetModuleHandle(NULL));
}


BOOL GroupBox::Create(LPCTSTR text, int x, int y, int nWidth, int nHeight, Wnd *parent_wnd, DWORD id)
{
	return (BOOL)Wnd::CreateEx(NULL, _T("BUTTON"), text, WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
								x, y, nWidth, nHeight, parent_wnd, (HMENU)id, ::GetModuleHandle(NULL));
}