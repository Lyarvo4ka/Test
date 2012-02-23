#include "stdafx.h"
#include "Synchro.h"
#include "SynchroDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSynchroDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, &CSynchroDlg::OnBnClickedStartButton)
	ON_CBN_DROPDOWN(IDC_FIRST_DRIVE_COMBO, &CSynchroDlg::OnCbnDropdownFirstDriveCombo)
	ON_CBN_DROPDOWN(IDC_SECOND_DRIVE_COMBO, &CSynchroDlg::OnCbnDropdownSecondDriveCombo)
	ON_CBN_DROPDOWN(IDC_TARGET_DRIVE_COMBO, &CSynchroDlg::OnCbnDropdownTargetDriveCombo)
	ON_CBN_SELCHANGE(IDC_FIRST_DRIVE_COMBO, &CSynchroDlg::OnCbnSelchangeFirstDriveCombo)
	ON_CBN_SELCHANGE(IDC_SECOND_DRIVE_COMBO, &CSynchroDlg::OnCbnSelchangeSecondDriveCombo)
	ON_CBN_SELCHANGE(IDC_TARGET_DRIVE_COMBO, &CSynchroDlg::OnCbnSelchangeTargetDriveCombo)
END_MESSAGE_MAP()

CSynchroDlg::CSynchroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynchroDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	first_drive_name = _T("");
	second_drive_name = _T("");
	target_drive_name = _T("");
	memset(&sd_params, 0x00, sizeof(sd_params));
}

CSynchroDlg::~CSynchroDlg(void)
{
	delete sd_params.first_drive;
	delete sd_params.second_drive;
	delete sd_params.target_drive;
}

void CSynchroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL ClearCB(CComboBox *cb)
{
	for (int i = cb->GetCount() - 1; i >= 0; --i )
		cb->DeleteString(i);
	return TRUE;
}

BOOL InitCB(CComboBox *cb)
{
	ClearCB(cb);
	if (LB_ERR  != cb->AddString(SELECT_DRIVE_STR))
		return TRUE; 
	return FALSE;
}

BOOL CSynchroDlg::IsSelect(const CString *drive_name)
{
	if ((first_drive_name == *drive_name)  ||
		(second_drive_name == *drive_name) ||
		(target_drive_name == *drive_name))
		return TRUE;
	else
		return FALSE;
}

BOOL CSynchroDlg::UpdateDrivesListInCB(CComboBox *cb)
{
	CString sel_drive_name = GetDriveNameFromCB(cb);
	InitCB(cb);
	CString drive_name = drives_mgr.GetFirstDriveName();
	while (drive_name != _T(""))
	{
		if (!IsSelect(&drive_name))
			cb->AddString(drive_name);
		drive_name = drives_mgr.GetNextDriveName();
	}
	return TRUE;
}

CString CSynchroDlg::GetDriveNameFromCB(CComboBox *cb)
{
	TCHAR buff[32] = {0};
	CString drive_name = _T("");
	if (CB_ERR != cb->GetLBText(cb->GetCurSel(), buff))
		if (0x00 != StrCmp(buff, SELECT_DRIVE_STR)) 
			return (drive_name = buff);
	return (drive_name = _T(""));
}

BOOL CSynchroDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	drives_mgr.UpdateList();

	CStatic *lba_static = (CStatic *)GetDlgItem(IDC_LBA_STATIC);
	lba_static->SetWindowText(_T("0"));

	CComboBox *first_drive_cb = (CComboBox *)GetDlgItem(IDC_FIRST_DRIVE_COMBO);
	CComboBox *second_drive_cb = (CComboBox *)GetDlgItem(IDC_SECOND_DRIVE_COMBO);
	CComboBox *target_drive_cb = (CComboBox *)GetDlgItem(IDC_TARGET_DRIVE_COMBO);

	if (first_drive_cb && second_drive_cb && target_drive_cb)
	{
		InitCB(first_drive_cb);
		InitCB(second_drive_cb);
		InitCB(target_drive_cb);
		first_drive_cb->SetCurSel(0);
		second_drive_cb->SetCurSel(0);
		target_drive_cb->SetCurSel(0);
	}
	CTime t(0x38A5B023);
	CString time_str = t.FormatGmt(_T("%Y %m %d %H:%M"));


	int x = 0;

	return TRUE;
}

void CSynchroDlg::OnPaint()
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

HCURSOR CSynchroDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSynchroDlg::OnBnClickedStartButton()
{
	if (first_drive_name == EMPTY_STR) {
		MessageBox(_T("Не выбран Drive 1"), _T("Error"));
		return;
	}
	if (second_drive_name == EMPTY_STR) {
		MessageBox(_T("Не выбран Drive 2"), _T("Error"));
		return;
	}
	if (target_drive_name == EMPTY_STR) {
		MessageBox(_T("Не выбран Target drive"), _T("Error"));
		return;
	}
	if (first_drive_name == second_drive_name) {
		MessageBox(_T("Drive 1 и Drive 2 должны быть разными"), _T("Error"));
		return;
	}
	if ((first_drive_name == target_drive_name) || (second_drive_name == target_drive_name)) {
		MessageBox(_T("Drive 1 и Drive 2 должны быть разными"), _T("Error"));
		return;
	}

	sd_params.dlg = this;
	sd_params.first_drive = new PhysicalDrive(first_drive_name);
	sd_params.second_drive = new PhysicalDrive(second_drive_name);
	sd_params.target_drive = new PhysicalDrive(target_drive_name);
	sd_params.update_lba_func = UpdateLBAStatic;

	thrd = AfxBeginThread(SynchronizeDrives, (LPVOID)&sd_params);
	if (!thrd) MessageBox(_T("Не удалоь создать рабочий поток"), _T("Error"));
}

void CSynchroDlg::OnCbnDropdownFirstDriveCombo()
{
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_FIRST_DRIVE_COMBO);
	if (cb)	UpdateDrivesListInCB(cb);
}

void CSynchroDlg::OnCbnDropdownSecondDriveCombo()
{
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_SECOND_DRIVE_COMBO);
	if (cb)	UpdateDrivesListInCB(cb);
}

void CSynchroDlg::OnCbnDropdownTargetDriveCombo()
{
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_TARGET_DRIVE_COMBO);
	if (cb)	UpdateDrivesListInCB(cb);
}

void CSynchroDlg::OnCbnSelchangeFirstDriveCombo()
{
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_FIRST_DRIVE_COMBO);
	if (cb)
		first_drive_name = GetDriveNameFromCB(cb);
}

void CSynchroDlg::OnCbnSelchangeSecondDriveCombo()
{
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_SECOND_DRIVE_COMBO);
	if (cb)
		second_drive_name = GetDriveNameFromCB(cb);
}

void CSynchroDlg::OnCbnSelchangeTargetDriveCombo()
{
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_TARGET_DRIVE_COMBO);
	if (cb)
		target_drive_name = GetDriveNameFromCB(cb);
}

BOOL IsZeroFill(BYTE *buff, DWORD size)
{
	for (DWORD i = 0; i < size; ++i)
	{
		if (buff[i] != 0x00)
		{
			return FALSE;
		}
	}
	return TRUE;
}

UINT __cdecl SynchronizeDrives(LPVOID param)
{
	SYNCHRONIZE_DRIVES_PARAMS *sd_params = (SYNCHRONIZE_DRIVES_PARAMS *)param;
	ULONGLONG lba = 0;
	//BYTE first_drive_sector[512] = {0};
	//BYTE second_drive_sector[512] = {0};
	BYTE first_drive_sector[256*512] = {0};
	BYTE second_drive_sector[256*512] = {0};
	DWORD rw = 0;
	DWORD rw1 = 0;
	DWORD rw2 = 0;
	CStatic *static_wnd = (CStatic *)sd_params->dlg->GetDlgItem(IDC_LBA_STATIC);
	CButton *start_btn = (CButton *)sd_params->dlg->GetDlgItem(IDC_START_BUTTON);
	CComboBox *first_drive_cb = (CComboBox *)sd_params->dlg->GetDlgItem(IDC_FIRST_DRIVE_COMBO);
	CComboBox *second_drive_cb = (CComboBox *)sd_params->dlg->GetDlgItem(IDC_SECOND_DRIVE_COMBO);
	CComboBox *target_drive_cb = (CComboBox *)sd_params->dlg->GetDlgItem(IDC_TARGET_DRIVE_COMBO);

	if (start_btn) start_btn->EnableWindow(FALSE);
	if (first_drive_cb) first_drive_cb->EnableWindow(FALSE);
	if (second_drive_cb) second_drive_cb->EnableWindow(FALSE);
	if (target_drive_cb) target_drive_cb->EnableWindow(FALSE);

	if (!sd_params->first_drive->Open(GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE)  ||
		!sd_params->second_drive->Open(GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE) ||
		!sd_params->target_drive->Open(GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE))
		return 1;

	//while ((512 == sd_params->first_drive->Read(first_drive_sector, 512)) &&
	//	   (512 == sd_params->second_drive->Read(second_drive_sector, 512)))
	//{
	//	if (!IsZeroFill(first_drive_sector, 512))
	//	{
	//		if (!sd_params->target_drive->Write(first_drive_sector, 512))
	//		{
	//			Exception e(_T("target_drive::Write"));
	//			e.ShowMessage();
	//			CString str = _T("Error");
	//			static_wnd->SetWindowText((LPCTSTR)str);
	//			goto _exit;
	//		}
	//	}
	//	else
	//	{
	//		if (!sd_params->target_drive->Write(second_drive_sector, 512))
	//		{
	//			Exception e(_T("target_drive::Write"));
	//			e.ShowMessage();
	//			CString str = _T("Error");
	//			static_wnd->SetWindowText((LPCTSTR)str);
	//			goto _exit;
	//		}
	//	}

	//	if (static_wnd) 
	//		sd_params->update_lba_func(static_wnd, lba);

	//	memset(first_drive_sector, 0x00, 512);
	//	memset(second_drive_sector, 0x00, 512);

	//	lba++;
	//}

	while(TRUE)
	{
		while ((rw1 = sd_params->first_drive->Read(first_drive_sector, 256*512)) &&
			  (rw2 = sd_params->second_drive->Read(second_drive_sector, 256*512)))
		{
			rw = (rw1 < rw2) ? rw1 : rw2;
			for (DWORD i = 0; (DWORD)(i*512) < rw; i++)
			{
				if (!IsZeroFill(&first_drive_sector[i*512], 512))
				{
					if (!sd_params->target_drive->Write(&first_drive_sector[i*512], 512))
					{
						Exception e(_T("target_drive::Write"));
						e.ShowMessage();
						goto _exit;
					}
				}
				else
				{
					if(!sd_params->target_drive->Write(&second_drive_sector[i*512], 512))
					{
						Exception e(_T("target_drive::Write"));
						e.ShowMessage();
						goto _exit;
					}
				}

				if (static_wnd) 
				{
					sd_params->update_lba_func(static_wnd, lba);
				}

				++lba;
			}
			if (rw1 != rw2) goto _exit; 
		}
	}

_exit:

	if (start_btn) start_btn->EnableWindow(TRUE);
	if (first_drive_cb) first_drive_cb->EnableWindow(TRUE);
	if (second_drive_cb) second_drive_cb->EnableWindow(TRUE);
	if (target_drive_cb) target_drive_cb->EnableWindow(TRUE);

	CString str;
	str.Format(_T("Complete: %I64d"), lba);
	static_wnd->SetWindowText((LPCTSTR)str);

	return 0;
}

UINT __cdecl UpdateLBAStatic(CStatic *static_wnd, ULONGLONG lba)
{
	CString str;
	str.Format(_T("%I64d"), lba);
	static_wnd->SetWindowText((LPCTSTR)str);
	
	return 0;
}