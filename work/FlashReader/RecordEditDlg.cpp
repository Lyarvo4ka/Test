#include "stdafx.h"
#include "FlashReader.h"
#include "RecordEditDlg.h"

IMPLEMENT_DYNAMIC(CRecordEditDlg, CDialog)

BEGIN_MESSAGE_MAP(CRecordEditDlg, CDialog)
	ON_FLASH_CHANGED(OnFlashChanged)
END_MESSAGE_MAP()

CRecordEditDlg::CRecordEditDlg(CWnd* pParent, CFlashDatabase& database, BOOL bNew)
: CDialog(CRecordEditDlg::IDD, pParent)
, m_FlashRecord(database)
, m_dlgParameters(NULL, &m_FlashRecord)
, m_bNew(bNew)
{
}

CRecordEditDlg::~CRecordEditDlg()
{
}

void CRecordEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CRecordEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect	rectTemp;
	GetDlgItem(IDC_FLASH_PARAMETERS)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgParameters.Create(this, rectTemp.TopLeft(), m_bNew);
	return TRUE;
}

LRESULT CRecordEditDlg::OnFlashChanged(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDOK)->EnableWindow(m_FlashRecord.IsEnable());
	return 0;
}

void CRecordEditDlg::OnOK()
{
	if (m_bNew && m_FlashRecord.m_OwnerBase.GetRecord(m_FlashRecord.m_strPartNumber))
	{
		AfxMessageBox(IDS_WRONG_PURTNUMBER, MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	CDialog::OnOK();
}
