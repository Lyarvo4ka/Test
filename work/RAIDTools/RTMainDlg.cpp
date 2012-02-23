#include "stdafx.h"
#include "RTApp.h"
#include "RTMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRTMainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

CRTMainDlg::CRTMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRTMainDlg::IDD, pParent), work_dir(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_RT_MAIN_ICON);
}

void CRTMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CRTMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	all_drives.Update();
	UpdatePhysicalDriveComboBox();
	UpdateMaxBlockSizeComboBox();
	UpdateAvaliableDrivesList();

	return TRUE;
}

void CRTMainDlg::UpdateAvaliableDrivesList(void)
{
	CListBox *list_box = (CListBox *)GetDlgItem(IDC_AV_PDRIVES_LIST);
	CStatic *static_ctrl = (CStatic *)GetDlgItem(IDC_AVALIABLE_DEVICES_STATIC);
	if (static_ctrl)
	{
		CString new_text;
		new_text.Format(_T("%s%d"), AVALIABLE_DEVICES_STATIC_TEXT, all_drives.GetCount());
		static_ctrl->SetWindowText((LPCTSTR)new_text);
	}
	if (list_box)
	{
		CString drive_name = all_drives.GetFirst();
		while (drive_name != _T(""))
		{
			list_box->AddString((LPCTSTR)drive_name);
			drive_name = all_drives.GetNext();
		}
	}
}
void CRTMainDlg::UpdatePhysicalDriveComboBox(void)
{
	CComboBox *combo_box = (CComboBox *)GetDlgItem(IDC_PHYSICAL_DRIVE_COMBO);
	if (combo_box)
	{
		CString drive_name = all_drives.GetFirst();
		while (drive_name != _T(""))
		{
			combo_box->AddString((LPCTSTR)drive_name);
			drive_name = all_drives.GetNext();
		}	
	}
}

void CRTMainDlg::UpdateMaxBlockSizeComboBox(void)
{
	CComboBox *combo_box = (CComboBox *)GetDlgItem(IDC_MAX_BLOCK_SZ_COMBO);
	if (combo_box)
	{
		CString drive_name = all_drives.GetFirst();
		while (drive_name != _T(""))
		{
			combo_box->AddString((LPCTSTR)drive_name);
			drive_name = all_drives.GetNext();
		}	
	}
}

void CRTMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CRTMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

