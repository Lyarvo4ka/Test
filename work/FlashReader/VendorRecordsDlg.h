#pragma once

#include "FlashDatabase.h"
#include "OxGridList.h"

class CVendorRecordsDlg : public CDialog
{
public:
	enum { IDD = IDD_VENDOR_RECORDS };

protected:
	CFlashDatabase&	m_Database;
	COXGridList		m_gridVendor;
	BOOL			m_bModified;

public:
	CVendorRecordsDlg(CWnd* pParent, CFlashDatabase& database);
	virtual ~CVendorRecordsDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnClickedVendorNew();
	afx_msg void OnClickedVendorRemove();

private:
	int InsertVendor(CVendorRecord* pVendor);

	DECLARE_DYNAMIC(CVendorRecordsDlg)
	DECLARE_MESSAGE_MAP()
};
