#include "stdafx.h"
#include "FlashReader.h"
#include "FlashInfoDlg.h"
#include "FlashRecord.h"

IMPLEMENT_DYNAMIC(CFlashInfoDlg, CDialog)

BEGIN_MESSAGE_MAP(CFlashInfoDlg, CDialog)
END_MESSAGE_MAP()

CFlashInfoDlg::CFlashInfoDlg(CWnd* pParent)
: CDialog(CFlashInfoDlg::IDD, pParent)
{
}

CFlashInfoDlg::~CFlashInfoDlg()
{
}

void CFlashInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CFlashInfoDlg::Create(CWnd* pParentWnd, CPoint ptWnd)
{
	if (!CDialog::Create(IDD, pParentWnd)) return FALSE;
	CRect	rectTemp;
	// Move window
	GetWindowRect(&rectTemp);
	rectTemp.MoveToXY(ptWnd);
	MoveWindow(&rectTemp);
	// Init controls
	ShowFlashInfo(NULL);
	// Show window
	ShowWindow(SW_SHOW);
	return TRUE;
}

void CFlashInfoDlg::OnOK()
{
	// Do not REMOVE!!!!
}

void CFlashInfoDlg::OnCancel()
{
	// Do not REMOVE!!!!
}

void CFlashInfoDlg::ShowFlashInfo(CFlashRecord* pRecord)
{
	CString	strTitle;
	if (pRecord)
	{
		CString strVendor = pRecord->GetStringVendor();
		strTitle = pRecord->m_strPartNumber;
		if (!strVendor.IsEmpty()) strTitle += (_T(", ") + strVendor);
	}
	SetDlgItemText(IDC_INFO_PARTNUMBER, strTitle);
	SetDlgItemText(IDC_INFO_ID, pRecord ? pRecord->GetStringSignature(pRecord->m_nID, pRecord->m_nMaskID) : _T(""));
	SetDlgItemText(IDC_INFO_SIZE, pRecord ? pRecord->GetStringSize() : _T(""));
	SetDlgItemText(IDC_INFO_GEOMETRY, pRecord ? pRecord->GetStringGeometry() : _T(""));
	SetDlgItemText(IDC_INFO_COMMAND, pRecord ? pRecord->GetStringCommand() : _T(""));
	SetDlgItemText(IDC_INFO_BUS, pRecord ? CFlashRecord::GetStringBusWidth(pRecord->m_nBusWidth) : _T(""));
	SetDlgItemText(IDC_INFO_SPEED, pRecord ? CFlashRecord::GetStringSpeed(pRecord->m_nSpeedAsyn) : _T(""));
	SetDlgItemText(IDC_INFO_VCC_IO, pRecord ? CFlashRecord::GetStringPowerIO(pRecord->m_nPowerIO) : _T(""));
	SetDlgItemText(IDC_INFO_ONFI, pRecord ? CFlashRecord::GetStringSupportONFI(pRecord->m_nONFI) : _T(""));
}
