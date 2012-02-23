#pragma once

#include "FlashDatabase.h"
#include "DumpEdit\DumpEdit.h"

class CFlashParametersDlg : public CDialog
{
public:
	enum { IDD = IDD_FLASH_PARAMETERS };

protected:
	CFlashRecord*	m_pFlash;
	CDumpEdit*		m_pEditID;
	BOOL			m_bNew;

public:
	CFlashParametersDlg(CWnd* pParent, CFlashRecord* pFlash);
	virtual ~CFlashParametersDlg();
	BOOL Create(CWnd* pParentWnd, CPoint ptWnd, BOOL bNew);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnComboVendor();
	afx_msg void OnComboCsChip();
	afx_msg void OnComboBus();
	afx_msg void OnComboCmdType();
	afx_msg void OnComboCmdCycle();
	afx_msg void OnComboFullAdrCycle();
	afx_msg void OnCombPageAdrCycle();
	afx_msg void OnComboSpeed();
	afx_msg void OnComboPowerIO();
	afx_msg void OnComboONFI();
	afx_msg void OnChangePartnumber();
	afx_msg void OnChangePageSize();
	afx_msg void OnChangePageBlock();
	afx_msg void OnChangeBlockLU();
	afx_msg void OnChangeCountLU();
	afx_msg void OnChangeID();
	afx_msg void OnChangeMask(UINT nID);

private:
	void SetSelection(CComboBox* pCombo, DWORD_PTR nData);
	void UpdateSize();
	
	DECLARE_DYNAMIC(CFlashParametersDlg)
	DECLARE_MESSAGE_MAP()
};
