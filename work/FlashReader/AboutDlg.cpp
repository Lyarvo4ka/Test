#include "StdAfx.h"
#include "FlashReader.h"
#include "ClassesExt\FileVersion.h"
#include "AboutDlg.h"

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CFileVersion	fileVer;
	GetDlgItem(IDC_APP_NAME)->SetWindowText(fileVer.GetStringProductName(0));
	GetDlgItem(IDC_APP_VERSION)->SetWindowText(fileVer.GetStringProductVersion(0));
	GetDlgItem(IDC_APP_COPYRIGHT)->SetWindowText(fileVer.GetStringLegalCopyright(0));
	CenterWindow(AfxGetApp()->GetMainWnd());
	return TRUE;
}
