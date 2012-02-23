#include "stdafx.h"
#include "FlashReader.h"
#include "MainFrm.h"
#include "FlashMapView.h"
#include "FlashReaderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_READER,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM
};

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndExt)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndExt)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_FILE_THREAD_MESSAGE(&CMainFrame::OnWorkThreadMessage)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndExt::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndExt::Dump(dc);
}
#endif //_DEBUG

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndExt::PreCreateWindow(cs)) return FALSE;
	cs.style &= ~FWS_PREFIXTITLE;
	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, CSize(398, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	// add the second splitter pane - an data view in column 1
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CFlashMapView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	m_wndSplitter.m_bCreated = TRUE;
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndExt::OnCreate(lpCreateStruct) == -1) return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}
	m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_READER), ID_INDICATOR_READER, SBPS_NORMAL/*SBPS_NOBORDERS*/, 310);
	// Set bitmap to default menu
	m_DefaultMenuExt.LoadToolBar(IDR_MENU_BMP);
	// Set system menu with extendet stile
	CImageList	ilMainIcon;
	ilMainIcon.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	ilMainIcon.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_SystemMenuExt.SetMenuTitle(GetTitle(), MFT_SUNKEN|MFT_LINE|MFT_CENTER|MFT_SIDE_TITLE, &ilMainIcon, 0);
	// Resize window
	CRect rectTemp;
	GetActiveView()->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	rectTemp.right *= 2;
	CalcWindowRect(rectTemp);
	MoveWindow(rectTemp);
	ShowReaderInfo(_T(""));
#ifdef USER_MODE
	CMenu*	pMainMenu = GetMenu();
	CMenu*	pSubMenu;
	for (UINT nPosMain = 0; nPosMain < pMainMenu->GetMenuItemCount(); nPosMain++)
	{
		pSubMenu = pMainMenu->GetSubMenu(nPosMain);
		if (!pSubMenu) continue;
		for (UINT nPos = 0; nPos < pSubMenu->GetMenuItemCount(); nPos++)
		{
			if (ID_DATABASE_IMPORT != pSubMenu->GetMenuItemID(nPos)) continue;
			pSubMenu->DeleteMenu(nPos, MF_BYPOSITION);
			break;
		}
	}
#endif
	return 0;
}

void CMainFrame::OnClose()
{
	CFrameWndExt::OnClose();
}

LRESULT CMainFrame::OnWorkThreadMessage(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == MessageThreadStart)
	{
		Invalidate();
		UpdateWindow();
	}
	((CFlashReaderDoc*)GetActiveDocument())->OnReadThreadMessage(wParam, lParam);
	return 0;
}

void CMainFrame::ShowReaderInfo(CString strInfo)
{
	if (!m_wndStatusBar.GetSafeHwnd()) return;
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_READER), strInfo);
}
