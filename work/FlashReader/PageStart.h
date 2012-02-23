#pragma once

#include "FlashParametersDlg.h"
#include "BaseRecordsDlg.h"

class CNewTaskDlg;

class CPageStart : public CPropertyPage
{
public:
	enum { IDD = IDD_PAGE_START };

protected:
	CImageList			m_ilState;
	CFlashRecord&		m_Flash;
	CFlashParametersDlg	m_FlashParametersDlg;
	CBaseRecordsDlg		m_FlashRecordsDlg;
	CNewTaskDlg*		m_pSheet;
	CStatic*			m_pStateCS[MAX_CS];
	ULONGLONG			m_nSignature;
	UINT				m_nSelectID;
	BOOL				m_bConnected;
	BOOL				m_bCheckSuccess;

public:
	CPageStart(CFlashRecord& flashRecord);
	virtual ~CPageStart();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	void SetNextButton();
	void SetCheckControls();
	afx_msg void OnDestroy();
	afx_msg void OnSelectChange(UINT nID);
	afx_msg void OnClickedBtnCheck();
	afx_msg LRESULT OnFlashChanged(WPARAM wParam, LPARAM lParam);

private:
	void SetSelection(CComboBox* pCombo, DWORD_PTR nData);

	DECLARE_DYNAMIC(CPageStart)
	DECLARE_MESSAGE_MAP()
public:
};
