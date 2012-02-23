#include "stdafx.h"
#include "FlashReader.h"
#include "BaseEditDlg.h"
#include "VendorRecordsDlg.h"

IMPLEMENT_DYNAMIC(CBaseEditDlg, CDialog)

BEGIN_MESSAGE_MAP(CBaseEditDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_NEW, &CBaseEditDlg::OnClickedNew)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CBaseEditDlg::OnClickedEdit)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CBaseEditDlg::OnClickedRemove)
	ON_BN_CLICKED(IDC_BTN_VENDORS, &CBaseEditDlg::OnClickedVendors)
	ON_FLASH_CHANGED(OnFlashChanged)
END_MESSAGE_MAP()

CBaseEditDlg::CBaseEditDlg(CFlashDatabase& flashDatabase, CWnd* pParent)
: CDialog(CBaseEditDlg::IDD, pParent)
, m_Database(flashDatabase)
, m_dlgRecords(NULL, flashDatabase)
{
}

CBaseEditDlg::~CBaseEditDlg()
{
}

void CBaseEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CBaseEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect	rectTemp;

	GetDlgItem(IDC_FLASH_INFO)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgFlashInfo.Create(this, rectTemp.TopLeft());
	GetDlgItem(IDC_BASE_RECORDS)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgRecords.Create(this, rectTemp.TopLeft());
	GetDlgItem(IDC_BTN_NEW)->EnableWindow((BOOL)m_Database.GetVendorCount());
	EnableButtons();
	return TRUE;
}

void CBaseEditDlg::OnOK()
{
	OnClose();
}

void CBaseEditDlg::OnCancel()
{
	// Do not REMOVE!!!!
}

void CBaseEditDlg::OnClose()
{
	m_Database.Save();
	CDialog::OnClose();
	CDialog::OnOK();
}

void CBaseEditDlg::OnClickedNew()
{
	m_dlgRecords.AddRecord();
	EnableButtons();
}

void CBaseEditDlg::OnClickedEdit()
{
	m_dlgRecords.EditRecord();
}


void CBaseEditDlg::OnClickedRemove()
{
	m_dlgRecords.RemoveRecord();
	EnableButtons();
}

void CBaseEditDlg::OnClickedVendors()
{
	CVendorRecordsDlg	dlg(this, m_Database);
	dlg.DoModal();
	GetDlgItem(IDC_BTN_NEW)->EnableWindow((BOOL)m_Database.GetVendorCount());
}

LRESULT CBaseEditDlg::OnFlashChanged(WPARAM wParam, LPARAM lParam)
{
	m_dlgFlashInfo.ShowFlashInfo((CFlashRecord*)lParam);
	return 0;
}

void CBaseEditDlg::EnableButtons()
{
	BOOL	bBtnEnable = (BOOL)m_Database.GetRecordCount();
	GetDlgItem(IDC_BTN_EDIT)->EnableWindow(bBtnEnable);
	GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(bBtnEnable);
}
