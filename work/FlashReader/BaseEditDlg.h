#pragma once

#include "BaseRecordsDlg.h"
#include "FlashInfoDlg.h"

class CBaseEditDlg : public CDialog
{
public:
	enum { IDD = IDD_BASE_EDIT };

private:
	CBaseRecordsDlg	m_dlgRecords;
	CFlashDatabase&	m_Database;
	CFlashInfoDlg	m_dlgFlashInfo;

public:
	CBaseEditDlg(CFlashDatabase& flashDatabase, CWnd* pParent = NULL);
	virtual ~CBaseEditDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnClickedNew();
	afx_msg void OnClickedEdit();
	afx_msg void OnClickedRemove();
	afx_msg void OnClickedVendors();
	afx_msg LRESULT OnFlashChanged(WPARAM wParam, LPARAM lParam);
	void EnableButtons();

	DECLARE_DYNAMIC(CBaseEditDlg)
	DECLARE_MESSAGE_MAP()
};
