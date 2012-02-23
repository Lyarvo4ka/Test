#pragma once

#include "FlashParametersDlg.h"
#include "BaseRecordsDlg.h"

class CFlashReaderDoc;

class CNewTaskDlg : public CDialog
{
public:
	enum { IDD = IDD_NEW_TASK };
	CFlashRecord		m_Flash;
	CString				m_strTaskName;

protected:
	CImageList			m_ilState;
	CFlashParametersDlg	m_FlashParametersDlg;
	CBaseRecordsDlg		m_FlashRecordsDlg;
	CFlashReaderDoc*	m_pOwner;
	CStatic*			m_pStateCS[MAX_CS];
	CString				m_strTask;
	ULONGLONG			m_nSignature;
	UINT				m_nSelectID;
	BOOL				m_bConnected;

public:
	CNewTaskDlg(CFlashReaderDoc* pOwner, CWnd* pParent = NULL);
	virtual ~CNewTaskDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditTask();
	afx_msg void OnSelectChange(UINT nID);
	afx_msg void OnClickedBtnCheck();
	afx_msg void OnClickedBtnReset();
	afx_msg LRESULT OnFlashChanged(WPARAM wParam, LPARAM lParam);

private:
	void SetComboSelection(CComboBox* pCombo, DWORD_PTR nData);
	void SetButtonOK();

	DECLARE_DYNAMIC(CNewTaskDlg)
	DECLARE_MESSAGE_MAP()
};
