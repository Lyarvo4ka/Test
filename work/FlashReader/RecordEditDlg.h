#pragma once

#include "FlashParametersDlg.h"

class CRecordEditDlg : public CDialog
{
public:
	enum { IDD = IDD_RECORD_EDIT };
	CFlashRecord		m_FlashRecord;

private:
	CFlashParametersDlg	m_dlgParameters;
	BOOL				m_bNew;

public:
	CRecordEditDlg(CWnd* pParent, CFlashDatabase& database, BOOL bNew);
	virtual ~CRecordEditDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg LRESULT OnFlashChanged(WPARAM wParam, LPARAM lParam);

	DECLARE_DYNAMIC(CRecordEditDlg)
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};
