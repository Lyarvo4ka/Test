#pragma once

#include "FlashInfoDlg.h"

class CNewTaskDlg;

class CPageFinish : public CPropertyPage
{
public:
	enum { IDD = IDD_PAGE_FINISH };

protected:
	CNewTaskDlg*	m_pSheet;
	CFlashInfoDlg	m_dlgFlashInfo;
	CString			m_strTask;
	BOOL			m_bFlashEnable;

public:
	CPageFinish();
	virtual ~CPageFinish();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	void SetButtons();

	DECLARE_DYNAMIC(CPageFinish)
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChangeEditTask();
};
