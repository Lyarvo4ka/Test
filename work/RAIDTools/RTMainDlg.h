#pragma once

#define AVALIABLE_DEVICES_STATIC_TEXT				_T("Available devices: ")
#define SELECTED_DEVICES_STATIC_TEXT				_T("Selected devices: ")

class CRTMainDlg : public CDialog
{
private:
	HICON m_hIcon;
	PDrivesMgr all_drives;
	CString work_dir;

	void UpdateAvaliableDrivesList(void);
	void UpdatePhysicalDriveComboBox(void);
	void UpdateMaxBlockSizeComboBox(void);

public:
	CRTMainDlg(CWnd* pParent = NULL);

protected:
	enum { IDD = IDD_RT_MAIN_DIALOG };
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
