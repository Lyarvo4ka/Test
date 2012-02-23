#include "stdafx.h"
#include "FlashReader.h"
#include "ReplaceRecordDlg.h"

IMPLEMENT_DYNAMIC(CReplaceRecordDlg, CDialog)

BEGIN_MESSAGE_MAP(CReplaceRecordDlg, CDialog)
END_MESSAGE_MAP()

CReplaceRecordDlg::CReplaceRecordDlg(CWnd* pParent)
: CDialog(CReplaceRecordDlg::IDD, pParent)
{
	m_pRecordSrc = NULL;
	m_pRecordDst = NULL;
}

CReplaceRecordDlg::~CReplaceRecordDlg()
{
}

void CReplaceRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CReplaceRecordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect	rectTemp;

	GetDlgItem(IDC_RECORD_DST)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgDst.Create(this, rectTemp.TopLeft());
	m_dlgDst.ShowFlashInfo(m_pRecordDst);
	GetDlgItem(IDC_RECORD_SRC)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgSrc.Create(this, rectTemp.TopLeft());
	m_dlgSrc.ShowFlashInfo(m_pRecordSrc);
	return TRUE;
}
