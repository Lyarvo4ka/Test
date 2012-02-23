#ifndef _STATIC
#define _STATIC

#include "main.h"

class WStatic : public Wnd
{
public:
	WStatic(void) {}
	~WStatic(void) {}

	BOOL WStatic::Create(LPCTSTR text, DWORD static_style, int x, int y, int nWidth, int nHeight, Wnd *parent, DWORD id)
	{
		DWORD style = static_style | WS_CHILD | WS_VISIBLE;
		if (Wnd::Create(_T("STATIC"), text, style, x, y, nWidth, nHeight, parent, (HMENU)id, ::GetModuleHandle(NULL)))
			return TRUE;
		else return FALSE;
	}

	BOOL WStatic::CreateTextStatic(LPCTSTR text, DWORD static_style, int x, int y, int nWidth, int nHeight, Wnd *parent, DWORD id)
	{
		return TRUE;
	}

	// The return value is a handle to the icon, or NULL if either the static control
	// has no associated icon or if an error occurred. 
	HICON GetIcon() {return (HICON)::SendMessage(GetHandle(), STM_GETICON, 0, 0);}

	// The return value is a handle to the image, if any; otherwise, it is NULL.
	HANDLE GetImage(DWORD image_type)
	{return (HANDLE)::SendMessage(GetHandle(), STM_GETIMAGE, (WPARAM)image_type, 0);}

	// The return value is a handle to the icon previously associated with the icon
	// control, or zero if an error occurs. 
	HICON SetIcon(HICON hicon) {return (HICON)::SendMessage(GetHandle(), STM_SETICON, (WPARAM)hicon, 0);}
	
	// The return value is a handle to the image previously associated with the static
	// control, if any; otherwise, it is NULL.
	HANDLE SetImage(DWORD image_type, HANDLE himage)
	{return (HANDLE)::SendMessage(GetHandle(), STM_SETIMAGE, (WPARAM)image_type, (LPARAM)himage);}
};

#endif _STATIC
