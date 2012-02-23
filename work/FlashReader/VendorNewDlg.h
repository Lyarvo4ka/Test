#pragma once

#include "FlashDatabase.h"

class CVendorNewDlg : public CDialog
{
public:
	enum { IDD = IDD_VENDOR_NEW };
	CString	m_strName;
	BYTE	m_nVID;

protected:
	CFlashDatabase&	m_Database;

public:
	CVendorNewDlg(CWnd* pParent, CFlashDatabase& database);
	virtual ~CVendorNewDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	
	DECLARE_DYNAMIC(CVendorNewDlg)
	DECLARE_MESSAGE_MAP()
public:
};
