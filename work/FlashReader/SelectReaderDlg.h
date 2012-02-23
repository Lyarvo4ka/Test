#pragma once

#include "Reader.h"

class CSelectReaderDlg : public CDialog
{
public:
	enum { IDD = IDD_SELECT_READER };
	CReaderList*			m_pListReader;
	CReader*			m_pReader;

protected:
	CTreeCtrl			m_treeDevices;
	CImageList			m_ilDevice;

public:
	CSelectReaderDlg(CWnd* pParent = NULL);
	virtual ~CSelectReaderDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblClkTreeDevices(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_DYNAMIC(CSelectReaderDlg)
	DECLARE_MESSAGE_MAP()
};
