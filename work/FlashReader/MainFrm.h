#pragma once

#include "WindowExt\WindowClasses.h"
#include "Splitter.h"

class CMainFrame : public CFrameWndExt
{
public:

protected:
	CStatusBar			m_wndStatusBar;
	CToolBar			m_wndToolBar;
	CSplitter			m_wndSplitter;

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	void ShowReaderInfo(CString strInfo);
	
protected:
	CMainFrame();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg LRESULT OnWorkThreadMessage(WPARAM wParam, LPARAM lParam);

private:

	DECLARE_DYNCREATE(CMainFrame)
	DECLARE_MESSAGE_MAP()
};
