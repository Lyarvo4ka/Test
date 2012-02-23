#ifndef _LISTBOX
#define _LISTBOX

#include "main.h"

class ListBox : public Wnd
{
public:
	ListBox(void) {}
	~ListBox(void) {}

	void Create(DWORD dwStyle, int x, int y, int nWidth, int nHeight, Wnd *parent, DWORD id)
	{
		dwStyle = dwStyle | WS_CHILD | WS_VISIBLE;
		Wnd::Create(_T("LISTBOX"), _T(""), dwStyle, x, y, nWidth, nHeight,
					parent, (HMENU)id, ::GetModuleHandle(NULL));
	}

	// Adds a string to a list box. The return value is the zero-based index of the string in the list
	// box. If an error occurs, the return value is LB_ERR. If there is insufficient space to store
	// the new string, the return value is LB_ERRSPACE. 
	LRESULT AddString(LPCTSTR str)
	{return ::SendMessage(GetHandle(), LB_ADDSTRING, (WPARAM)0, (LPARAM)str);}

	// Deletes a string in a list box. The return value is a count of the strings remaining in the list.
	// The return value is LB_ERR if the wParam parameter specifies an index greater than the number of
	// items in the list. 
	LRESULT DeleteString(WORD index)
	{return ::SendMessage(GetHandle(), LB_DELETESTRING, (WPARAM)index, (LPARAM)0);}
};

#endif
