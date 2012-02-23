#include "stdafx.h"
//#include "AnalyzerIDE.h"
#include "Splitter.h"

IMPLEMENT_DYNAMIC(CSplitter, CSplitterWnd)

BEGIN_MESSAGE_MAP(CSplitter, CSplitterWnd)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CSplitter::CSplitter()
{
	m_bCreated = FALSE;
}

CSplitter::~CSplitter()
{
}

CWnd* CSplitter::GetActivePane(int* pRow, int* pCol)
{
	ASSERT_VALID(this);

	// attempt to use active view of frame window
	CWnd* pView = NULL;
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	pView = pFrameWnd->GetActiveView();
	// failing that, use the current focus
	if (pView == NULL) pView = GetFocus();
	return pView;
}

void CSplitter::StartTracking(int ht)
{	// Don't remove - fixed splitter
}

void CSplitter::OnMouseMove(UINT nFlags, CPoint point)
{	// No change cursor on fixed splitter
	point.SetPoint(0,0);
	CSplitterWnd::OnMouseMove(nFlags, point);
}
