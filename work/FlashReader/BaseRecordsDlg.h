#pragma once

#include "FlashDatabase.h"
#include "OxGridList.h"

class CBaseRecordsDlg : public CDialog
{
public:
	enum { IDD = IDD_BASE_RECORDS };

protected:
	CFlashDatabase&	m_database;
	COXGridList		m_gridRecord;
	CString			m_strFilterTotalSize;
	DWORD			m_nFilterPageSize;
	DWORD			m_nFilterVendor;
	DWORD			m_nFilterBus;
	DWORD			m_nFilterCS;
	ULONGLONG		m_nFilterID;

public:
	CBaseRecordsDlg(CWnd* pParent, CFlashDatabase& database);
	virtual ~CBaseRecordsDlg();
	BOOL Create(CWnd* pParentWnd, CPoint ptWnd);
	CFlashRecord* GetCurrentRecord();
	CFlashRecord* AddRecord(CFlashRecord* pRecord = NULL);
	void EditRecord();
	void RemoveRecord();
	void SetFilterCheck(CFlashRecord* pRecord);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnChangedListRecords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeFilterVendor();
	afx_msg void OnChangeFilterTotalSize();
	afx_msg void OnChangeFilterPageSize();
	afx_msg void OnChangeFilterBusWidth();
	afx_msg void OnChangeFilterCS();

private:
	void SetFilters(BOOL bSaveSelection = TRUE);
	void ShowRecords();

	DECLARE_DYNAMIC(CBaseRecordsDlg)
	DECLARE_MESSAGE_MAP()
};
