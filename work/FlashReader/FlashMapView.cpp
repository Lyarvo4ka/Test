#include "stdafx.h"
#include "FlashReader.h"
#include "FlashMapView.h"

IMPLEMENT_DYNCREATE(CFlashMapView, CScrollView)

BEGIN_MESSAGE_MAP(CFlashMapView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_TOOLTIP_DISPLAY(OnToolTipDisplay)
END_MESSAGE_MAP()

CFlashMapView::CFlashMapView()
{
	CBitmap			bmp;
	m_ilPageState.Create(8, 8, ILC_COLORDDB|ILC_MASK, 0, 1);
	bmp.LoadBitmap(IDB_PAGE_STATE);
	m_ilPageState.Add(&bmp, RGB(255, 0, 255));
	bmp.DeleteObject();
	::ImageList_GetIconSize(m_ilPageState, (int*)&m_sizeCell.cx, (int*)&m_sizeCell.cy);
	m_nPagePerBlock = 0;
	m_nPagePerCS = 0;
	m_nPageCount = 0;
}

CFlashMapView::~CFlashMapView()
{
}

#ifdef _DEBUG
void CFlashMapView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFlashMapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFlashReaderDoc* CFlashMapView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlashReaderDoc)));
	return (CFlashReaderDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CFlashMapView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	if (!CScrollView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext)) return FALSE;
	SetScrollSizes(MM_TEXT, CSize(10, 10));
	// create and initilyze tooltip object
	m_toolTip.Create(this, false);
	m_toolTip.SetNotify();
	m_toolTip.SetDirection(PPTOOLTIP_LEFTEDGE_BOTTOM);
	PPTOOLTIP_INFO ti;
	//ti.nIDTool = IDC_STATISTIC_WND;
	ti.rectBounds.SetRectEmpty();
	ti.sTooltip = "";
	ti.nDirection = 0;
	ti.nBehaviour = PPTOOLTIP_MULTIPLE_SHOW/* | PPTOOLTIP_TRACKING_MOUSE*/;
	ti.nMask = PPTOOLTIP_MASK_BEHAVIOUR;
	m_toolTip.AddTool(this, ti);
	return TRUE;
}

BOOL CFlashMapView::PreTranslateMessage(MSG* pMsg)
{
	m_toolTip.RelayEvent(pMsg);
	return CScrollView::PreTranslateMessage(pMsg);
}

BOOL CFlashMapView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CFlashMapView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetMapMode(MM_TEXT);
}


void CFlashMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
}

void CFlashMapView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	WORD			nType = LOWORD(lHint);
	if (nType == UpdateStepProgress)
	{
		ULONGLONG nPageStart = m_nStepStartPage + *(ULONG*)pHint;
		ULONGLONG nPageEnd = nPageStart + HIWORD(lHint);
		nPageStart = __max(nPageStart, m_nFirstVisiblePage);
		nPageEnd = __min(nPageEnd, m_nFirstUnvisiblePage);
		if (nPageStart < nPageEnd)
		{
			CRect	rectUpdate(CPoint(0, 0), m_sizeCell);
			nPageStart -= m_nFirstVisiblePage;
			nPageEnd -= m_nFirstVisiblePage;
			for (ULONGLONG nPage = nPageStart; nPage < nPageEnd; nPage++)
			{
				rectUpdate.MoveToXY((nPage % m_nPagePerRaw) * m_sizeCell.cx, ((int)(nPage / m_nPagePerRaw)) * m_sizeCell.cy);
				InvalidateRect(rectUpdate, FALSE);
			}
			UpdateWindow();
		}
	}
	else if (nType == UpdateStepStart)
	{
		m_nStepStartPage = m_nPagePerCS * GetDocument()->m_nCurrentCS;
	}
	else if (nType == UpdateStepEnd)
	{
		Invalidate();
		UpdateWindow();
	}
	else if (nType == UpdateInitial)
	{
		m_nPagePerBlock = GetDocument()->m_Flash.m_nPagePerBlock;
		m_nPagePerCS = GetDocument()->m_Flash.GetPagePerCS();
		m_nPagePerLU = GetDocument()->m_Flash.GetPagePerLU();
		m_nPageCount = GetDocument()->m_Flash.GetPageCount();
		SetScreenVariables();
		SetScrollPos(SB_VERT, 0);
		Invalidate();
	}
}

void CFlashMapView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	if (cx == 0 || cy == 0) return;
	CClientDC	clientDC(this);	
	GetClientRect(&m_rectScr);
	m_nPagePerRaw = m_rectScr.Width() / m_sizeCell.cx;
	clientDC.SetMapMode(MM_TEXT);
	clientDC.DPtoLP(&m_rectScr);
	SetScreenVariables();
}


void CFlashMapView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if ((nSBCode == SB_THUMBTRACK || nSBCode == SB_THUMBPOSITION))
	{
		SCROLLINFO   info;
		GetScrollInfo(SB_VERT, &info, SIF_ALL);
		nPos = info.nTrackPos;
	}
	else if (nSBCode == SB_ENDSCROLL)
	{
		SCROLLINFO   info;
		GetScrollInfo(SB_VERT, &info, SIF_ALL);
		CalculateViseblePage(info.nPos);
	}
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CFlashMapView::OnToolTipDisplay(WPARAM wParam, LPARAM lParam)
{
	NM_PPTOOLTIP_DISPLAY*	pNotify = (NM_PPTOOLTIP_DISPLAY*)lParam;
	CPoint					ptTooltip(*pNotify->pt);

	if (m_nPageCount)
	{
		ScreenToClient(&ptTooltip);
		ptTooltip.y += GetScrollPos(SB_VERT);
		ptTooltip.y /= m_sizeCell.cy;
		ptTooltip.x /= m_sizeCell.cx;
		if (ptTooltip.x < m_nPagePerRaw)
		{
			ULONG	nPageGlobal = ptTooltip.y * m_nPagePerRaw + ptTooltip.x;
			ULONG	nPageInCS = nPageGlobal % (ULONG)m_nPagePerCS;
			ULONG	nPageInLU = nPageInCS % (ULONG)m_nPagePerLU;
			pNotify->ti->sTooltip.Format(_T("Global page %d\n(CE %d, LU %d, Block %d, Page %d)"),
										 nPageGlobal,
										 nPageGlobal / (ULONG)m_nPagePerCS,	//CE
										 nPageInCS / (ULONG)m_nPagePerLU,		//LU
										 nPageInLU / m_nPagePerBlock,	//Block
										 nPageInLU % m_nPagePerBlock);	//Page
		}
	}
	return 0;
}

void CFlashMapView::SetScreenVariables()
{
	LONGLONG	nRawCount;
	LONG		nWidth;

	nRawCount = m_nPageCount / m_nPagePerRaw;
	if (m_nPageCount % m_nPagePerRaw) nRawCount++;
	m_nRawPerScr = m_rectScr.Height() / m_sizeCell.cy;
	if (m_rectScr.Height() % m_sizeCell.cy) m_nRawPerScr++;
	CalculateViseblePage(GetScrollPos(SB_VERT));
	nWidth = m_nPagePerRaw * m_sizeCell.cx - ::GetSystemMetrics(SM_CXVSCROLL);
	SetScrollSizes(MM_TEXT,											//Map Mode
				   CSize(nWidth, (int)(nRawCount * m_sizeCell.cy)),	//Total
				   CSize(nWidth, m_nRawPerScr * m_sizeCell.cy),		//Page
				   CSize(nWidth, m_sizeCell.cy));					//Line
}

void CFlashMapView::CalculateViseblePage(INT nPos)
{
	m_nFirstVisiblePage = (nPos / m_sizeCell.cy) * m_nPagePerRaw;
	m_nFirstUnvisiblePage = m_nFirstVisiblePage + m_nRawPerScr * m_nPagePerRaw;
}

void CFlashMapView::OnDraw(CDC* pDC)
{
	//TRACE("OnDraw\n");
	CDC*		pDrawDC = pDC;
	CDC			dcMem;
	CBitmap		bmpMem;
	CBitmap*	pOldBitmap;
	CRect		rectClipBoxLP;
	CRect		rectClipBoxDP;
	
	// only paint the rect that needs repainting
	pDC->GetClipBox(rectClipBoxLP);		
	if (!pDC->IsPrinting())
	{	// draw to offscreen bitmap for fast looking repaints
		dcMem.CreateCompatibleDC(pDC);
		rectClipBoxDP = rectClipBoxLP;
		pDC->LPtoDP(rectClipBoxDP);
		rectClipBoxDP.NormalizeRect();
		bmpMem.CreateCompatibleBitmap(pDC, rectClipBoxDP.Width(), rectClipBoxDP.Height());
		pOldBitmap = dcMem.SelectObject(&bmpMem);
		// set up mapping mode and viewport origin
		dcMem.SetMapMode(pDC->GetMapMode());
		dcMem.SetWindowOrg(pDC->GetWindowOrg());
		dcMem.SetViewportOrg(pDC->GetViewportOrg());
		// offset origin more because bitmap is just piece of the whole drawing
		dcMem.OffsetViewportOrg(-rectClipBoxDP.left, -rectClipBoxDP.top);
		dcMem.SetBrushOrg(rectClipBoxDP.left % 8, rectClipBoxDP.top % 8);
		// might as well clip to the same rectangle
		dcMem.IntersectClipRect(rectClipBoxLP);
		pDrawDC = &dcMem;
		::FillRect(pDrawDC->m_hDC, &rectClipBoxLP, CBrush(GetSysColor(COLOR_3DFACE)));
	}
	pDrawDC->SetBkMode(TRANSPARENT);
	if (m_nPageCount)
	{
		DrawClipBox(pDrawDC); 
	}
	else
	{
		CPoint	pt;
		CString	strNoFlash;
		GetClientRect(&rectClipBoxLP);
		pDrawDC->DPtoLP(&rectClipBoxLP);
		pt = rectClipBoxLP.CenterPoint();
		pt.Offset(-rectClipBoxDP.left, -rectClipBoxDP.top);
		strNoFlash.LoadString(IDS_NO_FLASH);
		pDrawDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
		pDrawDC->TextOut(pt.x, pt.y, strNoFlash);
	}
	if (pDrawDC != pDC)
	{	//BitBlt the Bitmap in MM_TEXT Mode
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0, 0);
		pDC->SetMapMode(MM_TEXT);
		dcMem.SetViewportOrg(0, 0);
		dcMem.SetWindowOrg(0,0);
		dcMem.SetMapMode(MM_TEXT);
		pDC->BitBlt(rectClipBoxDP.left, rectClipBoxDP.top, rectClipBoxDP.Width(), rectClipBoxDP.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pOldBitmap);
	}
}

void CFlashMapView::DrawClipBox(CDC* pDC)
{
	//TRACE("DrawClipBox\n");
	CFlashMap&	FlashMap(GetDocument()->m_FlashMap);
	CRect		rectClipBox;
	CRect		rectClient;
	CPoint		ptDraw;
	LONG		nRowBegin;
	LONG		nRowEnd;
	LONG		nColBegin;
	LONG		nColEnd;
	ULONG		nPage;
	ULONG		nPageOffset;
	LONG		nDrawOffsetX;

	pDC->GetClipBox(&rectClipBox);
	if (rectClipBox.IsRectEmpty()) return;
	//TRACE("DrawClipBox %d, %d, %d, %d\n", rectClipBox.left, rectClipBox.right, rectClipBox.top, rectClipBox.bottom);
	if (pDC->IsPrinting())
	{	// Draw header
	}
	else
	{
		GetClientRect(&rectClient);
		rectClient.MoveToY(GetScrollPos(SB_VERT));
		nRowBegin = rectClipBox.top / m_sizeCell.cy;
		nRowEnd = rectClipBox.bottom / m_sizeCell.cy;
		nColBegin = rectClipBox.left / m_sizeCell.cx;
		nColEnd = __min((rectClipBox.right / m_sizeCell.cx), m_nPagePerRaw-1);
		ptDraw.y = nRowBegin * m_sizeCell.cy;
		nDrawOffsetX = nColBegin * m_sizeCell.cx;
		nPageOffset = nRowBegin * m_nPagePerRaw + nColBegin;
		for (LONG nRow = nRowBegin; nRow <= nRowEnd; nRow++)
		{
			ptDraw.x = nDrawOffsetX;
			nPage = nPageOffset;
			for (LONG nCol = nColBegin; nCol <= nColEnd; nCol++)
			{
				if (nPage < m_nPageCount) m_ilPageState.Draw(pDC, FlashMap.GetPageState(nPage), ptDraw, ILD_NORMAL);
				ptDraw.x += m_sizeCell.cx;
				nPage++;
			}
			ptDraw.y += m_sizeCell.cy;
			nPageOffset += m_nPagePerRaw;
		}
	}
}
