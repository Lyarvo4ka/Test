#pragma once

#include "FlashReaderDoc.h"
#include "ToolTipEx\\ToolTipEx.h"

class CFlashMapView : public CScrollView
{
protected:
	CToolTipEx		m_toolTip;
	CImageList		m_ilPageState;
	CSize			m_sizeCell;
	ULONG			m_nPagePerBlock;
	ULONGLONG		m_nPagePerCS;
	ULONGLONG		m_nPagePerLU;
	ULONGLONG		m_nPageCount;
	ULONGLONG		m_nStepStartPage;
	//screen variables
	CRect			m_rectScr;
	LONG			m_nPagePerRaw;
	LONG			m_nRawPerScr;
	ULONG			m_nFirstVisiblePage;
	ULONG			m_nFirstUnvisiblePage;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CFlashReaderDoc* GetDocument() const;

protected:
	CFlashMapView();
	virtual ~CFlashMapView();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnToolTipDisplay(WPARAM wParam, LPARAM  lParam);

private:
	void SetScreenVariables();
	void CalculateViseblePage(INT nPos);
	void DrawClipBox(CDC* pDC);

	DECLARE_DYNCREATE(CFlashMapView)
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FlashReaderView.cpp
inline CFlashReaderDoc* CFlashMapView::GetDocument() const { return reinterpret_cast<CFlashReaderDoc*>(m_pDocument); }
#endif
