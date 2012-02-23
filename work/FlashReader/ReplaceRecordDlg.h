#pragma once

#include "FlashInfoDlg.h"
#include "FlashRecord.h"

class CReplaceRecordDlg : public CDialog
{
public:
	enum { IDD = IDD_UPDATE_RECORD };
	CFlashRecord*	m_pRecordSrc;
	CFlashRecord*	m_pRecordDst;

protected:
	CFlashInfoDlg	m_dlgSrc;
	CFlashInfoDlg	m_dlgDst;

public:
	CReplaceRecordDlg(CWnd* pParent = NULL);
	virtual ~CReplaceRecordDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_DYNAMIC(CReplaceRecordDlg)
	DECLARE_MESSAGE_MAP()
};
