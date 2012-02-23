#pragma once

#include "Reader.h"

class CNANDReaderDlg : public CDialog
{
private:
	CReader reader;

public:
	enum { IDD = IDD_NANDREADER_DIALOG };

	CNANDReaderDlg(CWnd* pParent = NULL);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
