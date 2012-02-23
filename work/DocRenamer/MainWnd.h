#ifndef _MAINWND
#define _MAINWND

#include "stdafx.h"
#include "Resource.h"
#include "RichEdit.h"

using namespace Gdiplus;

class MainWnd : public W32Lib::Wnd
{
	typedef enum ControlsID
	{
		kNull,
		kSourceFolderEB,
		kSelectBtn,
		kStartBtn,
		kProgressBar
	};
private:
	RichEdit source_re;
public:
	MainWnd(void);
	~MainWnd(void);

	void Create(LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HMENU hMenu);

	virtual LRESULT OnCreate(LPCREATESTRUCT pcs);
	virtual LRESULT OnEraseBackground(HDC hdc);
	virtual void OnPaint(HDC hdc);
	virtual void OnClose();
	virtual void OnDestroy();
	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT nType, int width, int height);
};

#endif //_MAINWND