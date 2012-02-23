#pragma once

#include "afxcmn.h"
#include "FlashReaderDoc.h"
#include "FlashInfoDlg.h"

class CFlashReaderView : public CFormView
{
public:
	enum{ IDD = IDD_FLASHREADER_FORM };

protected:
	CFlashInfoDlg	m_dlgFlashInfo;
	CProgressCtrl	m_progressStep;
	CImageList		m_ilState;
	CStatic*		m_pPageState[PageStateCount];
	CStatic*		m_pSubTaskState[ModeCount+1][MAX_CS];
	CStatic*		m_pModeState[ModeCount+1];
	CStatic*		m_pCheckState;
	UINT_PTR		m_nTimerID;
	ULONGLONG		m_nPagePerCS;
	ULONGLONG		m_nStepStartPage;
	BOOL			m_bFirstUpdate;

public:
	virtual ~CFlashReaderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CFlashReaderDoc* GetDocument() const;

protected:
	CFlashReaderView();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeMode(UINT nID);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

private:
	void StringToLog(CString strLog);
	void ClearLog();
	void InitialUpdate();
	void SetModeState(INT nMode, INT nState);
	void SetSubTaskState(INT nMode, INT nCS, INT nState);
	void ShowStateCheck(BOOL bRecordEnable);
	void ShowStateTask(BOOL bRecordEnable);
	void ShowPageStateCount();
	void EnableModeButtons(BOOL bEnable);
	void StartTimer();
	void StopTimer();

	DECLARE_DYNCREATE(CFlashReaderView)
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FlashReaderView.cpp
inline CFlashReaderDoc* CFlashReaderView::GetDocument() const { return reinterpret_cast<CFlashReaderDoc*>(m_pDocument); }
#endif

