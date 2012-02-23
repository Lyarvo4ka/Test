#include "stdafx.h"
#include "RawerApp.h"
#include "Box.h"
#include "MainWnd.h"


template <class SingleObj>
SingleObj *Singleton<SingleObj>::object = NULL;


IMPLEMENT_DYNAMIC(CBox, CWnd)

BEGIN_MESSAGE_MAP(CBox, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CBox::CBox(CWnd *parentWnd, const RECT &wndRect)
{
	WNDCLASS wc;
	memset(&wc, 0x00, sizeof(wc));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hInstance = ::AfxGetInstanceHandle();
	wc.hIcon = ::LoadIcon(NULL, IDI_ASTERISK);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CBOX_CLASS_NAME;

	if (!AfxRegisterClass(&wc))
		AfxThrowResourceException();

	if (!CreateEx(WS_EX_CONTROLPARENT | WS_EX_TOPMOST, CBOX_CLASS_NAME, CBOX_WND_NAME,
				  WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, wndRect, parentWnd, 0, NULL))
		AfxThrowResourceException();
}

void CBox::OnPaint()
{
	CWnd::OnPaint();

	CRect wndRect;
	GetClientRect(&wndRect);

	CPaintDC dc(this);
	CDC memDC;
	if (!memDC.CreateCompatibleDC(&dc)) 
		return;

	CBitmap bkGndBmp;
	if(!bkGndBmp.CreateCompatibleBitmap(&dc, wndRect.Width(), wndRect.Height()))
		return;

	if(!memDC.SelectObject(&bkGndBmp))
		return;

	DrawBackground(&memDC);

	dc.BitBlt(0, 0, wndRect.Width(), wndRect.Height(), &memDC, 0, 0, SRCCOPY);
}

void CBox::DrawBackground(CDC* pDC)
{
	CRect wndRect;
	GetClientRect(&wndRect);

	CBrush bGndBr(RGB(243,246,251));
	CBrush frameBr(RGB(224,223,215));
	CBrush frameBr1(RGB(152,181,226));

	pDC->FrameRect(&wndRect, &frameBr);
	wndRect.DeflateRect(1,1);
	pDC->FillRect(&wndRect, &bGndBr);
	pDC->FrameRect(&wndRect, &frameBr1);
}
