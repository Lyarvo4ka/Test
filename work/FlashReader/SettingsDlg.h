#pragma once

#include "afxcmn.h"
#include "OXDropEdit.h"
#include "OXEdit.h"

#define STEP_COUNT_DEFAULT	100
#define LIMIT_DEFAULT		50
#define DELTA_DEFAULT		25

class CBrowseDirectory: public COXDropEdit<COXEdit>
{
public:

protected:

private:

public:
	CBrowseDirectory();
	virtual ~CBrowseDirectory();

protected:
	virtual void OnDropButton();
	
	DECLARE_DYNAMIC(CBrowseDirectory)
	DECLARE_MESSAGE_MAP()
};

class CSettingsDlg : public CDialog
{
public:
	enum { IDD = IDD_SETTINGS };
	CString				m_strDirectory;
	UINT				m_nLimit;
	UINT				m_nDelta;
	UINT				m_nStepCount;
	UINT				m_nLogSize;
	BOOL				m_bLogEnable;
	BOOL				m_bSeparateCS;

protected:
	CSliderCtrl			m_sldLimit;
	CSliderCtrl			m_sldDelta;
	CBrowseDirectory	m_editDirectory;

public:
	CSettingsDlg(CWnd* pParent = NULL);
	virtual ~CSettingsDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnDefaults();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClickedCheckLogEnable();
	void SetAll();
	void ShowValue(CSliderCtrl& sldCtrl, UINT nValueID, INT nValue);
	void UpdateLimitRange();
	void UpdateDeltaRange();

	DECLARE_DYNAMIC(CSettingsDlg)
	DECLARE_MESSAGE_MAP()
public:
};
