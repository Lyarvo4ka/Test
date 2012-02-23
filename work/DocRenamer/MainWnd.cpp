#include "stdafx.h"
#include "MainWnd.h"

MainWnd::MainWnd(void)
{
}

MainWnd::~MainWnd(void)
{
}

void MainWnd::Create(LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HMENU hMenu)
{
	Wnd::Create(WND_CLASS_NAME, lpWindowName, dwStyle, x, y, nWidth, nHeight,
				NULL, hMenu, ::GetModuleHandle(NULL));
}


LRESULT MainWnd::OnCreate(LPCREATESTRUCT pcs)
{
	try
	{
		source_re.Create(_T("tetet"), NULL, 10, 10, 50, 100, this, kSourceFolderEB);



		CenterWindow();
		SetIcon(::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WORDICON)), 0);

		return 0;
	}
	catch(Exception &e)
	{
		e.ShowMessage();
		return  -1;
	}
}

LRESULT MainWnd::OnSize(UINT nType, int width, int height)
{
	return 0;
}

void MainWnd::OnPaint(HDC hdc)
{
	// ¬се строки - в кодировке Unicode
	//WCHAR welcome[]=L"Welcome, GDI+ !";

	// —оздаем контекст рисовани€ и устанавливаем 
	// пиксельную систему координат
	//Graphics g(hdc);
	//g.SetPageUnit(UnitPixel);
	//RECT rc;
	//GetClientRect(&rc);
	//RectF bounds(0, 0, float(rc.right), float(rc.bottom));

	//RECT rect;
	//::GetClientRect(hWnd, &rect);

	// «агружаем фоновое изображение и раст€гиваем его на все окно         
	//Image bg(L"BACKGRND.gif");
	//g.DrawImage(&bg, bounds);

	// —оздаем кисть с градиентом на все окно и полупрозрачностью         
	//LinearGradientBrush brush
	//(bounds, Color(130, 255, 0, 0), Color(255, 0, 0, 255), 
	//        LinearGradientModeBackwardDiagonal);

	//SolidBrush text_brush(Color(255, 0,0,0));
	//SolidBrush brush(Color(10, 200, 0, 0));

	// √отовим формат и параметры шрифта
	//StringFormat format;
	//format.SetAlignment(StringAlignmentCenter);
	//format.SetLineAlignment(StringAlignmentCenter);
	//Font font(L"Tahoma", 8, FontStyleRegular/*FontStyleBold*/);

	// ¬ыводим текст приветстви€, длина -1 означает,.
	// что строка заканчиваетс€ нулем    
	//g.DrawString(welcome, -1, &font, bounds, &format, &text_brush);

	//g.FillRectangle(&brush, bounds);
}

LRESULT MainWnd::OnEraseBackground(HDC hdc)
{
	Graphics g(hdc);
	g.SetPageUnit(UnitPixel);
	RECT rc; GetClientRect(&rc);
	RectF bounds(0, 0, (REAL)(rc.right), (REAL)(rc.bottom));
	SolidBrush brush(Color(255,236,233,216));
	g.FillRectangle(&brush, bounds);

	return 1;
}

void MainWnd::OnClose()
{
	DestroyWindow();
}

void MainWnd::OnDestroy()
{
	::PostQuitMessage(0);
}
LRESULT MainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		if (LOWORD(wParam) == 1)
		{
			if (HIWORD(wParam) == BN_CLICKED) {}
		}
	}
	return 0;
}
