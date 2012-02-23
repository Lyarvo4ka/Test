#pragma once

class CFlashRecord;

class CFlashInfoDlg : public CDialog
{
public:
	enum { IDD = IDD_FLASH_INFO };

public:
	CFlashInfoDlg(CWnd* pParent = NULL);
	virtual ~CFlashInfoDlg();
	BOOL Create(CWnd* pParentWnd, CPoint ptWnd);
	void ShowFlashInfo(CFlashRecord* pRecord);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_DYNAMIC(CFlashInfoDlg)
	DECLARE_MESSAGE_MAP()
};
