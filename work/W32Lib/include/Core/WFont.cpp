#include "WFont.h"

BOOL WFont::CreateFont(HDC hdc, LPCTSTR font_name, int nHeight, DWORD style)
{
	if (hfont) DeleteFont();

	int	fnWeight = FW_NORMAL;
	if (style & Bold) fnWeight = FW_BOLD;

	DWORD fdwItalic = (DWORD)FALSE;
	if (style & Italic) fdwItalic = (DWORD)TRUE;

	DWORD fdwUnderline = FALSE;
	if (style & Underline) fdwUnderline = TRUE;

	DWORD fdwStrikeOut = FALSE;
	if (style & StrikeOut) fdwStrikeOut = TRUE;

	if (hdc)
	{
		int px_Y = ::GetDeviceCaps(hdc, LOGPIXELSY);
		nHeight = -::MulDiv(nHeight, px_Y, 72);
	}

	hfont = ::CreateFont(nHeight, 0, 0, 0, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font_name);
	return (BOOL)hfont;
}

BOOL WFont::ChooseFontDlg(void)
{
	if (hfont) DeleteFont();

	CHOOSEFONT cf; 
	LOGFONT lf; 

	cf.lStructSize = sizeof(CHOOSEFONT); 
	cf.hwndOwner = (HWND)NULL; 
	cf.hDC = (HDC)NULL; 
	cf.lpLogFont = &lf; 
	cf.iPointSize = 0; 
	cf.Flags = CF_EFFECTS|CF_SCREENFONTS; 
	cf.rgbColors = RGB(0,0,0); 
	cf.lCustData = 0L; 
	cf.lpfnHook = (LPCFHOOKPROC)NULL; 
	cf.lpTemplateName = (LPCTSTR )NULL; 
	cf.hInstance = (HINSTANCE) NULL; 
	cf.lpszStyle = (LPTSTR)NULL; 
	cf.nFontType = SCREEN_FONTTYPE; 
	cf.nSizeMin = 0; 
	cf.nSizeMax = 0; 

	if (::ChooseFont(&cf))
	{
		hfont = ::CreateFontIndirect(cf.lpLogFont);
		return (BOOL)hfont;
	}
	else
		return FALSE;
}


