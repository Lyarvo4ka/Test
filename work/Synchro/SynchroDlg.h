#pragma once
#include "W32Lib.h"
#include "list"
#include "shlwapi.h"

#define EMPTY_STR				_T("")
#define SELECT_DRIVE_STR		_T("Select drive")

class CSynchroDlg;

typedef UINT (__cdecl *UPDATE_LBA_STATIC_FUNC)(CStatic *static_wnd, ULONGLONG lba);

typedef struct _SYNCHRONIZE_DRIVES_PARAMS {
	CSynchroDlg *dlg;
	PhysicalDrive *first_drive;
	PhysicalDrive *second_drive;
	PhysicalDrive *target_drive;
	UPDATE_LBA_STATIC_FUNC update_lba_func;
} SYNCHRONIZE_DRIVES_PARAMS;

UINT __cdecl SynchronizeDrives(LPVOID param);
UINT __cdecl UpdateLBAStatic(CStatic *static_wnd, ULONGLONG lba);

class DriveMgr 
{
private:
	enum {kMaxPhysicalDriveNumber = 32};
	list<CString> drive_lst;
	list<CString>::iterator it;

	list<CString>::iterator Find(const CString *drive_name)
	{
		if (drive_name)
		{
			list<CString>::iterator i;
			for (i = drive_lst.begin(); i != drive_lst.end(); i++)
				if (*i == *drive_name)
					return i;
		}
		return drive_lst.end();
	}


public:
	void UpdateList(void)
	{
		drive_lst.clear();
		for (DWORD num = 0; num < kMaxPhysicalDriveNumber; ++num)
		{
			PhysicalDrive *drive = new PhysicalDrive(num);
			if (drive->Open(GENERIC_READ/*|GENERIC_WRITE*/, FILE_SHARE_READ/*|FILE_SHARE_WRITE*/))
			{
				CString drive_name = drive->GetName();
				drive_lst.push_back(drive_name);
			}
			else 
			{
				DWORD err = ::GetLastError();
				int x = 0;
			}
			delete drive;
		}
	}

	DWORD GetCount(void) {return drive_lst.size();}

	CString GetFirstDriveName(void)
	{
		CString empty_str = _T("");
		it = drive_lst.begin();
		if (it != drive_lst.end())
			return *it;
		else
			return empty_str;
	}

	CString GetNextDriveName(void)
	{
		CString empty_str = _T("");
		if (it != drive_lst.end()) 
			if (++it != drive_lst.end())
				return *it;
		return empty_str;
	}
};

class CSynchroDlg : public CDialog
{
private:
	enum { IDD = IDD_SYNCHRO_DIALOG };
	HICON m_hIcon;
	CString first_drive_name;
	CString second_drive_name;
	CString target_drive_name;
	DriveMgr drives_mgr;
	CWinThread *thrd;
	SYNCHRONIZE_DRIVES_PARAMS sd_params;

public:
	CSynchroDlg(CWnd* pParent = NULL);
	~CSynchroDlg(void);
	BOOL IsSelect(const CString *drive_name);
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnCbnDropdownFirstDriveCombo();
	afx_msg void OnCbnDropdownSecondDriveCombo();
	afx_msg void OnCbnDropdownTargetDriveCombo();
	afx_msg void OnCbnSelchangeFirstDriveCombo();
	afx_msg void OnCbnSelchangeSecondDriveCombo();
	afx_msg void OnCbnSelchangeTargetDriveCombo();

protected:
	BOOL UpdateDrivesListInCB(CComboBox *cb);
	CString GetDriveNameFromCB(CComboBox *cb);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
