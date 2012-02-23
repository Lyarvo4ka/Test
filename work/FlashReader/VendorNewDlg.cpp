#include "stdafx.h"
#include "FlashReader.h"
#include "VendorNewDlg.h"

IMPLEMENT_DYNAMIC(CVendorNewDlg, CDialog)

BEGIN_MESSAGE_MAP(CVendorNewDlg, CDialog)
END_MESSAGE_MAP()

CVendorNewDlg::CVendorNewDlg(CWnd* pParent, CFlashDatabase& database)
: CDialog(CVendorNewDlg::IDD, pParent)
, m_Database(database)
{
}

CVendorNewDlg::~CVendorNewDlg()
{
}

void CVendorNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CVendorNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CEdit*)GetDlgItem(IDC_EDIT_VENDOR_ID))->SetLimitText(2);
	SetDlgItemText(IDC_EDIT_VENDOR_ID, _T("00"));
	return TRUE;
}

BOOL CVendorNewDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == GetDlgItem(IDC_EDIT_VENDOR_ID)->GetSafeHwnd() && pMsg->message == WM_CHAR)
	{
		UCHAR	nChar = (UCHAR)pMsg->wParam;
		if (nChar > 0x19 && nChar < 0x30) return TRUE;
		if (nChar > 0x39 && nChar < 0x41) return TRUE;
		if (nChar > 0x46 && nChar < 0x61) return TRUE;
		if (nChar > 0x66) return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CVendorNewDlg::OnOK()
{
	CString			strTemp;
	BOOL			bPresent = FALSE;
	CVendorRecord*	pVendor;

	GetDlgItemText(IDC_EDIT_VENDOR_ID, strTemp);
	swscanf_s(strTemp, _T("%x"), &m_nVID);
	GetDlgItemText(IDC_EDIT_VENDOR_NAME, m_strName);
	m_strName.Trim();
	if ((m_nVID == 0x00) || m_strName.IsEmpty())
	{
		AfxMessageBox(IDS_WRONG_VENDOR_INPUT, MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	for (POSITION nPosVendor = m_Database.GetVendorHeadPosition(); nPosVendor != NULL; )
	{
		pVendor = m_Database.GetVendorNext(nPosVendor);
		if ((pVendor->m_nID == m_nVID) || (pVendor->m_strVendor == m_strName))
		{
			strTemp.Format(IDS_WRONG_VENDOR, m_nVID, m_strName);
			AfxMessageBox(strTemp, MB_OK|MB_ICONEXCLAMATION);
			return;
		}
	}
	CDialog::OnOK();
}
