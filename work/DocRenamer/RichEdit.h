#ifndef _RICHEDIT
#define _RICHEDIT

class RichEdit : public Wnd
{
private:
	HMODULE hrelibrary;
public:
	RichEdit(void) {}
	~RichEdit(void) {}

	void Create(LPCTSTR lpWindowName, DWORD re_style, int x, int y, int nWidth, int nHeight, Wnd *parent, DWORD id)
	{
		hrelibrary = ::LoadLibrary(_T("Riched20.dll"));
		if (!hrelibrary)
			throw W32Lib::Exception(_T("RichEdit::Create():\n"), ::GetLastError());

		re_style = re_style | WS_CHILD | WS_VISIBLE;
		Wnd::CreateEx(0, _T("RICHEDIT_CLASS"), lpWindowName, re_style, x, y, nWidth, nHeight,
					parent, (HMENU)id, ::GetModuleHandle(NULL));
	}
};
#endif //_RICHEDIT