#pragma once

class CNumberDlgAutoProxy;



class CNumberDlg : public CDialog
{
	DECLARE_DYNAMIC(CNumberDlg);
	friend class CNumberDlgAutoProxy;

protected:
	CNumberDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

public:
	enum { IDD = IDD_NUMBER_DIALOG };

	CNumberDlg(CWnd* pParent = NULL);
	virtual ~CNumberDlg();

	afx_msg void OnBnClickedOpenSrcBtn();
	afx_msg void OnBnClickedStartButton();
};
