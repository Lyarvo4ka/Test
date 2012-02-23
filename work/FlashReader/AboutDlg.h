#pragma once

class CAboutDlg : public CDialog
{
public:
	enum { IDD = IDD_ABOUTBOX };

public:
	CAboutDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
