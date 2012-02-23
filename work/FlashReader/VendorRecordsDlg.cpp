#include "stdafx.h"
#include "FlashReader.h"
#include "VendorRecordsDlg.h"
#include "VendorNewDlg.h"

#define VID_COLUMN_WHIDTH	50

const TCHAR* m_strColumnsName[2] =
{
	_T("Vendor name"),
	_T("VID")
};


IMPLEMENT_DYNAMIC(CVendorRecordsDlg, CDialog)

BEGIN_MESSAGE_MAP(CVendorRecordsDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_VENDOR_NEW, &CVendorRecordsDlg::OnClickedVendorNew)
	ON_BN_CLICKED(IDC_BTN_VENDOR_REMOVE, &CVendorRecordsDlg::OnClickedVendorRemove)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

CVendorRecordsDlg::CVendorRecordsDlg(CWnd* pParent, CFlashDatabase& database)
: CDialog(CVendorRecordsDlg::IDD, pParent)
, m_Database(database)
{
	m_bModified = FALSE;
}

CVendorRecordsDlg::~CVendorRecordsDlg()
{
	
}

void CVendorRecordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VENDORS, m_gridVendor);
}

BOOL CVendorRecordsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect			rectTemp;

	m_gridVendor.GetClientRect(rectTemp);
	m_gridVendor.InsertColumn(0, m_strColumnsName[0], LVCFMT_CENTER, rectTemp.Width() - ::GetSystemMetrics(SM_CXVSCROLL) - VID_COLUMN_WHIDTH);
	m_gridVendor.InsertColumn(1, m_strColumnsName[1], LVCFMT_CENTER, VID_COLUMN_WHIDTH);
	m_gridVendor.SetGridLines(TRUE, RGB(0, 0, 128));
	m_gridVendor.SetResizing(FALSE);
	m_gridVendor.SetSortable(TRUE);
	m_gridVendor.SetShowSel(TRUE);
	m_gridVendor.LockWindowUpdate();
	for (POSITION nPosVendor = m_Database.GetVendorHeadPosition(); nPosVendor != NULL; )
	{
		InsertVendor(m_Database.GetVendorNext(nPosVendor));
	}
	m_gridVendor.UnlockWindowUpdate();
	m_gridVendor.SetCurSel(0);
	return TRUE;
}

void CVendorRecordsDlg::OnClickedVendorNew()
{
	CVendorNewDlg	dlg(this, m_Database);
	if (IDOK == dlg.DoModal())
	{
		CVendorRecord*	pVendor = new CVendorRecord;
		pVendor->m_nID = dlg.m_nVID;
		pVendor->m_strVendor = dlg.m_strName;
		m_Database.AddVendor(pVendor);
		m_gridVendor.SetCurSel(InsertVendor(pVendor));
		m_bModified = TRUE;
	}
}

void CVendorRecordsDlg::OnClickedVendorRemove()
{
	CVendorRecord*	pVendor;
	CFlashRecord*	pRecord;
	int				nItem = m_gridVendor.GetCurSel();
	
	if (-1 == nItem) return;
	pVendor = (CVendorRecord*)m_gridVendor.GetItemData(nItem);
	for (POSITION nPosRecord = m_Database.GetRecordHeadPosition(); nPosRecord != NULL; )
	{
		pRecord =  m_Database.GetRecordNext(nPosRecord);
		if (pRecord->m_nVendorID == pVendor->m_nID)
		{
			AfxMessageBox(IDS_WRONG_VENDOR_REMOVE, MB_OK|MB_ICONEXCLAMATION);
			return;
		}
	}
	m_Database.RemoveVendor(pVendor);
	delete pVendor;
	m_gridVendor.DeleteItem(nItem);
	m_gridVendor.SetCurSel(0);
	m_bModified = TRUE;
}

int CVendorRecordsDlg::InsertVendor(CVendorRecord* pVendor)
{
	CString	strID;
	int		nItem = m_gridVendor.InsertItem(m_gridVendor.GetItemCount(), pVendor->m_strVendor);
	strID.Format(_T("%02X"), pVendor->m_nID);
	m_gridVendor.SetItemText(nItem, 1, strID);
	m_gridVendor.SetItemData(nItem, (DWORD_PTR)pVendor);
	return nItem;
}

void CVendorRecordsDlg::OnOK()
{
	OnClose();
}

void CVendorRecordsDlg::OnCancel()
{
	// Do not REMOVE!!!!
}

void CVendorRecordsDlg::OnClose()
{
	m_gridVendor.DeleteAllItems();
	CDialog::OnClose();
	m_bModified ? CDialog::OnOK() : CDialog::OnCancel();
}
