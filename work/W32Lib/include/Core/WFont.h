#ifndef _WFONT
#define _WFONT

#include "Global\Global.h"

using namespace W32Lib;

namespace W32Lib
{
	class WFont
	{
	private:
		HFONT hfont;
	public:
		enum
		{
			Normal		= 0,
			Bold		= 1,
			Italic		= 1 << 1,
			Underline	= 1 << 2,
			StrikeOut	= 1 << 3,
		};

		WFont(void) : hfont(NULL) {}
		~WFont(void) {DeleteFont();}

		BOOL CreateFont(HDC hdc, LPCTSTR font_name, int nHeight, DWORD style = WFont::Normal);
		BOOL ChooseFontDlg(void);

		HFONT GetHandle(void) {return hfont;};
		BOOL DeleteFont(void)
		{
			BOOL ret = ::DeleteObject(hfont);
			hfont = NULL;
			return ret;
		}
	};
}

#endif //_WFONT
