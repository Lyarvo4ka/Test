#include "stdafx.h"
#include "MainWnd.h"

IMPLEMENT_DYNAMIC(MainWnd, CWnd)

BEGIN_MESSAGE_MAP(MainWnd, CWnd)
	ON_CBN_SELENDOK(DRIVE_CB_ID, OnDriveCbSelEndOk)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

MainWnd::MainWnd() : font(NULL), source_box(NULL), file_type_box(NULL), drive_cb(NULL)
{
	if (!CreateEx(WS_EX_CONTROLPARENT, Global::strMainWndClass, theApp.m_pszAppName,
					WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0, 400, 400, NULL, NULL))
		AfxThrowResourceException();
	if (!InitializeWnd())
		AfxThrowResourceException();
}

MainWnd::~MainWnd()
{
	delete font;
	delete source_box;
	delete file_type_box;
	delete drive_cb;

	delete tstBox;
	delete tstBox2;
}

BOOL MainWnd::InitializeWnd(void)
{
	try 
	{
		font = new CFont;
		if (font->CreateFont(
			13,							// nHeight
			0,							// nWidth
			0,							// nEscapement
			0,							// nOrientation
			FW_NORMAL,					// nWeight
			FALSE,						// bItalic
			FALSE,						// bUnderline
			0,							// cStrikeOut
			DEFAULT_CHARSET,			// nCharSet
			OUT_DEFAULT_PRECIS,			// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,			// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			_T("Tahoma")))				// lpszFacename
		{
			SetFont(font);
		}
		else
		{
			delete font;
			font = NULL;
		}
		
		return CreateControls();

	}
	catch (std::bad_alloc)
	{
		return FALSE;
	}
}

BOOL MainWnd::CreateControls(void)
{
	RECT wnd_rect;
	GetClientRect(&wnd_rect);

	try
	{
		//RECT source_box_rect;
		//source_box_rect.left = wnd_rect.left + BORDER;
		//source_box_rect.top = wnd_rect.top + BORDER;
		//source_box_rect.right = wnd_rect.right - BORDER;
		//source_box_rect.bottom = source_box_rect.top + 100;

		//source_box = new CStatic;
		//if (!source_box->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME | SS_LEFT, source_box_rect, this))
		//	return FALSE;


		//RECT source_box_rect;
		//source_box_rect.left = wnd_rect.left + BORDER;
		//source_box_rect.top = wnd_rect.top + BORDER;
		//source_box_rect.right = wnd_rect.right - BORDER;
		//source_box_rect.bottom = source_box_rect.top + 100;

		//tstBox = new CBox(this, source_box_rect);

		//RECT file_type_box_rect;
		//file_type_box_rect.left = wnd_rect.left + BORDER;
		//file_type_box_rect.top = source_box_rect.bottom + BORDER;
		//file_type_box_rect.right = wnd_rect.right - BORDER;
		//file_type_box_rect.bottom = wnd_rect.bottom - BORDER; 

		//tstBox2 = new CBox(this, file_type_box_rect);

		//file_type_box = new CStatic;
		//if (!file_type_box->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME | SS_LEFT, file_type_box_rect, this))
		//	return FALSE;


		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
}

afx_msg void MainWnd::OnDriveCbSelEndOk()
{
	int x = 0;
}




void MainWnd::OnPaint()
{
	CPaintDC dc(this);

}


void MainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	


}

int MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
