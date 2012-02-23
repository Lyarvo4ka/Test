#include "stdafx.h"
#include "FlashReader.h"
#include "BaseRecordsDlg.h"
#include "RecordEditDlg.h"

#define	COLUMN_COUNT	4
#define	COLUMN_WHIDTH	60
#define	NO_FILTER_VALUE	0xFFFF
#define	NO_FILTER_STR	_T(" *ALL")
#define	NO_FILTER_ID	(ULONGLONG)-1

const TCHAR* m_strColumns[COLUMN_COUNT] =
{
	_T("Partnumber"),
	_T("Size"),
	_T("CE"),
	_T("Bus")
};

IMPLEMENT_DYNAMIC(CBaseRecordsDlg, CDialog)

BEGIN_MESSAGE_MAP(CBaseRecordsDlg, CDialog)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RECORDS, &CBaseRecordsDlg::OnChangedListRecords)
	ON_CBN_SELCHANGE(IDC_FILTER_VENDOR, &CBaseRecordsDlg::OnChangeFilterVendor)
	ON_CBN_SELCHANGE(IDC_FILTER_TOTAL_SIZE, &CBaseRecordsDlg::OnChangeFilterTotalSize)
	ON_CBN_SELCHANGE(IDC_FILTER_PAGE_SIZE, &CBaseRecordsDlg::OnChangeFilterPageSize)
	ON_CBN_SELCHANGE(IDC_FILTER_BUS, &CBaseRecordsDlg::OnChangeFilterBusWidth)
	ON_CBN_SELCHANGE(IDC_FILTER_CS, &CBaseRecordsDlg::OnChangeFilterCS)
END_MESSAGE_MAP()

CBaseRecordsDlg::CBaseRecordsDlg(CWnd* pParent, CFlashDatabase& database)
: CDialog(CBaseRecordsDlg::IDD, pParent)
, m_database(database)
{
}

CBaseRecordsDlg::~CBaseRecordsDlg()
{
}

void CBaseRecordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECORDS, m_gridRecord);
}

BOOL CBaseRecordsDlg::Create(CWnd* pParentWnd, CPoint ptWnd)
{
	if (!CDialog::Create(IDD, pParentWnd)) return FALSE;

	CRect			rectTemp;
	INT				nFirstColumn;

	// Move window
	GetWindowRect(&rectTemp);
	rectTemp.MoveToXY(ptWnd);
	MoveWindow(&rectTemp);
	// Init m_gridRecord
	m_gridRecord.GetClientRect(rectTemp);
	nFirstColumn = (rectTemp.Width() - ::GetSystemMetrics(SM_CXVSCROLL)) - (COLUMN_COUNT - 1) * COLUMN_WHIDTH;
	for (INT i = 0; i < COLUMN_COUNT; i++)
	{
		m_gridRecord.InsertColumn(i, m_strColumns[i], LVCFMT_CENTER, i ? COLUMN_WHIDTH : nFirstColumn);
	}
	m_gridRecord.SetGridLines(TRUE, RGB(0, 0, 128));
	m_gridRecord.SetResizing(FALSE);
	m_gridRecord.SetSortable(TRUE);
	m_gridRecord.SetShowSel(TRUE);
	// Init Filters
	SetFilters(FALSE);
	m_nFilterID = NO_FILTER_ID;
	// Show records
	ShowRecords();
	// Show window
	ShowWindow(SW_SHOW);
	return TRUE;
}

void CBaseRecordsDlg::OnOK()
{
	// Do not REMOVE!!!!
}

void CBaseRecordsDlg::OnCancel()
{
	// Do not REMOVE!!!!
}

void CBaseRecordsDlg::OnDestroy()
{
	m_gridRecord.DeleteAllItems();
	CDialog::OnDestroy();
}

void CBaseRecordsDlg::OnChangedListRecords(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV && (pNMLV->uNewState==(LVIS_SELECTED|LVIS_FOCUSED)) && (pNMLV->uChanged==LVIF_STATE))
	{
		GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_RECORD_CHANGED, (LPARAM)m_gridRecord.GetItemData(pNMLV->iItem));
	}
	*pResult = 0;
}

void CBaseRecordsDlg::OnChangeFilterVendor()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_FILTER_VENDOR);
	m_nFilterVendor = pCombo->GetItemData(pCombo->GetCurSel());
	ShowRecords();
}

void CBaseRecordsDlg::OnChangeFilterTotalSize()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_FILTER_TOTAL_SIZE);
	pCombo->GetLBText(pCombo->GetCurSel(), m_strFilterTotalSize);
	ShowRecords();
}

void CBaseRecordsDlg::OnChangeFilterPageSize()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_FILTER_PAGE_SIZE);
	m_nFilterPageSize = pCombo->GetItemData(pCombo->GetCurSel());
	ShowRecords();
}

void CBaseRecordsDlg::OnChangeFilterBusWidth()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_FILTER_BUS);
	m_nFilterBus = pCombo->GetItemData(pCombo->GetCurSel());
	ShowRecords();
}

void CBaseRecordsDlg::OnChangeFilterCS()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_FILTER_CS);
	m_nFilterCS = pCombo->GetItemData(pCombo->GetCurSel());
	ShowRecords();
}

CFlashRecord* CBaseRecordsDlg::GetCurrentRecord()
{
	int nInd = m_gridRecord.GetCurSel();
	return (nInd == -1) ? NULL : (CFlashRecord*)m_gridRecord.GetItemData(nInd);
}

CFlashRecord* CBaseRecordsDlg::AddRecord(CFlashRecord* pRecord)
{
	CRecordEditDlg	dlg(this, m_database, TRUE);
	if (pRecord) dlg.m_FlashRecord.Copy(pRecord);
	if (IDOK != dlg.DoModal()) return NULL;
	CFlashRecord*	pRecordNew = new CFlashRecord(m_database);
	pRecordNew->Copy(&dlg.m_FlashRecord);
	m_database.AddRecord(pRecordNew);
	SetFilters();
	ShowRecords();
	return pRecordNew;
}

void CBaseRecordsDlg::EditRecord()
{
	int	nInd = m_gridRecord.GetCurSel();
	if (nInd == -1) return;
	CFlashRecord*	pRecord = (CFlashRecord*)m_gridRecord.GetItemData(nInd);
	CRecordEditDlg	dlg(this, m_database, FALSE);
	dlg.m_FlashRecord.Copy(pRecord);
	if (IDOK != dlg.DoModal()) return;
	pRecord->Copy(&dlg.m_FlashRecord);
	SetFilters();
	ShowRecords();
}

void CBaseRecordsDlg::RemoveRecord()
{
	int	nInd = m_gridRecord.GetCurSel();
	if (nInd == -1) return;
	if (IDNO == AfxMessageBox(IDS_PROMPT_REMOVE, MB_YESNO|MB_ICONQUESTION)) return;
	CFlashRecord*	pRecord = (CFlashRecord*)m_gridRecord.GetItemData(nInd);
	m_database.RemoveRecord(pRecord);
	SetFilters();
	ShowRecords();
	delete pRecord;
}

void CBaseRecordsDlg::SetFilters(BOOL bSaveSelection)
{
	CFlashRecord*	pRecord;
	CVendorRecord*	pVendor;
	CComboBox*		pComboVendor = (CComboBox*)GetDlgItem(IDC_FILTER_VENDOR);
	CComboBox*		pComboTotalSize = (CComboBox*)GetDlgItem(IDC_FILTER_TOTAL_SIZE);
	CComboBox*		pComboPageSize = (CComboBox*)GetDlgItem(IDC_FILTER_PAGE_SIZE);
	CComboBox*		pComboBus = (CComboBox*)GetDlgItem(IDC_FILTER_BUS);
	CComboBox*		pComboCS = (CComboBox*)GetDlgItem(IDC_FILTER_CS);
	CString			strVendor;
	CString			strTotalSize;
	CString			strPageSize;
	CString			strBus;
	CString			strCS;
	CString			strTemp;
	int				nInd;

	if (bSaveSelection)
	{
		pComboVendor->GetLBText(pComboVendor->GetCurSel(), strVendor);
		pComboTotalSize->GetLBText(pComboTotalSize->GetCurSel(), strTotalSize);
		pComboPageSize->GetLBText(pComboPageSize->GetCurSel(), strPageSize);
		pComboBus->GetLBText(pComboBus->GetCurSel(), strBus);
		pComboCS->GetLBText(pComboCS->GetCurSel(), strCS);
	}
	pComboVendor->ResetContent();
	pComboVendor->SetItemData(pComboVendor->AddString(NO_FILTER_STR), NO_FILTER_VALUE);
	pComboTotalSize->ResetContent();
	pComboTotalSize->SetItemData(pComboTotalSize->AddString(NO_FILTER_STR), NO_FILTER_VALUE);
	pComboPageSize->ResetContent();
	pComboPageSize->SetItemData(pComboPageSize->AddString(NO_FILTER_STR), NO_FILTER_VALUE);
	pComboBus->ResetContent();
	pComboBus->SetItemData(pComboBus->AddString(NO_FILTER_STR), NO_FILTER_VALUE);
	pComboCS->ResetContent();
	pComboCS->SetItemData(pComboCS->AddString(NO_FILTER_STR), NO_FILTER_VALUE);
	for (POSITION nPosRecord = m_database.GetRecordHeadPosition(); nPosRecord != NULL; )
	{
		pRecord =  m_database.GetRecordNext(nPosRecord);
		strTemp.Empty();
		for (POSITION nPosVendor = m_database.GetVendorHeadPosition(); nPosVendor != NULL; )
		{
			pVendor = m_database.GetVendorNext(nPosVendor);
			if (pVendor->m_nID != pRecord->m_nVendorID) continue;
			strTemp = pVendor->m_strVendor;
			break;
		}
		if (CB_ERR == pComboVendor->FindStringExact(-1, strTemp))
		{
			pComboVendor->SetItemData(pComboVendor->AddString(strTemp), pRecord->m_nVendorID);
		}
		strTemp = pRecord->GetStringSize();
		if (CB_ERR == pComboTotalSize->FindStringExact(-1, strTemp))
		{
			pComboTotalSize->SetItemData(pComboTotalSize->AddString(strTemp), 0);
		}
		strTemp = pRecord->GetStringPageSize();
		if (CB_ERR == pComboPageSize->FindStringExact(-1, strTemp))
		{
			pComboPageSize->SetItemData(pComboPageSize->AddString(strTemp), pRecord->m_nPageSize);
		}
		strTemp = CFlashRecord::GetStringBusWidth(pRecord->m_nBusWidth);
		if (CB_ERR == pComboBus->FindStringExact(-1, strTemp))
		{
			pComboBus->SetItemData(pComboBus->AddString(strTemp), pRecord->m_nBusWidth);
		}
		strTemp.Format(_T("%d CE"), pRecord->m_nCountCS);
		if (CB_ERR == pComboCS->FindStringExact(-1, strTemp))
		{
			pComboCS->SetItemData(pComboCS->AddString(strTemp), pRecord->m_nCountCS);
		}
	}
	nInd = pComboVendor->FindStringExact(-1, strVendor);
	if (CB_ERR == nInd) nInd = 0;
	m_nFilterVendor = pComboVendor->GetItemData(nInd);
	pComboVendor->SetCurSel(nInd);
	nInd = pComboTotalSize->FindStringExact(-1, strTotalSize);
	if (CB_ERR == nInd) nInd = 0;
	pComboTotalSize->GetLBText(nInd, m_strFilterTotalSize);
	pComboTotalSize->SetCurSel(nInd);
	nInd = pComboPageSize->FindStringExact(-1, strPageSize);
	if (CB_ERR == nInd) nInd = 0;
	m_nFilterPageSize = pComboPageSize->GetItemData(nInd);
	pComboPageSize->SetCurSel(nInd);
	nInd = pComboBus->FindStringExact(-1, strBus);
	if (CB_ERR == nInd) nInd = 0;
	m_nFilterBus = pComboBus->GetItemData(nInd);
	pComboBus->SetCurSel(nInd);
	nInd = pComboCS->FindStringExact(-1, strCS);
	if (CB_ERR == nInd) nInd = 0;
	m_nFilterCS = pComboCS->GetItemData(nInd);
	pComboCS->SetCurSel(nInd);
}

void CBaseRecordsDlg::SetFilterCheck(CFlashRecord* pRecord)
{
	CComboBox*		pComboVendor = (CComboBox*)GetDlgItem(IDC_FILTER_VENDOR);
	CComboBox*		pComboTotalSize = (CComboBox*)GetDlgItem(IDC_FILTER_TOTAL_SIZE);
	CComboBox*		pComboPageSize = (CComboBox*)GetDlgItem(IDC_FILTER_PAGE_SIZE);
	CComboBox*		pComboBus = (CComboBox*)GetDlgItem(IDC_FILTER_BUS);
	CComboBox*		pComboCS = (CComboBox*)GetDlgItem(IDC_FILTER_CS);

	m_nFilterID = pRecord ? pRecord->m_nID : NO_FILTER_ID;
	m_nFilterCS = pRecord ? pRecord->m_nCountCS : NO_FILTER_VALUE;
	pComboCS->SetCurSel(pRecord ? pRecord->m_nCountCS : 0);
	m_nFilterBus = pRecord ? pRecord->m_nBusWidth : NO_FILTER_VALUE;;
	pComboBus->SetCurSel(pRecord ? pComboBus->FindStringExact(-1, CFlashRecord::GetStringBusWidth(pRecord->m_nBusWidth)) : 0);
	m_nFilterVendor = NO_FILTER_VALUE;
	pComboVendor->SetCurSel(0);
	m_strFilterTotalSize = NO_FILTER_STR;
	pComboTotalSize->SetCurSel(0);
	m_nFilterPageSize = NO_FILTER_VALUE;
	pComboPageSize->SetCurSel(0);
	ShowRecords();
}

void CBaseRecordsDlg::ShowRecords()
{
	CFlashRecord*	pRecord;
	int				nInd;
	CString			strCS;

	m_gridRecord.LockWindowUpdate();
	m_gridRecord.DeleteAllItems();
	for (POSITION nPosRecords = m_database.GetRecordHeadPosition(); nPosRecords != NULL; )
	{
		pRecord = m_database.GetRecordNext(nPosRecords);
		if ((m_nFilterID != NO_FILTER_ID) && ((m_nFilterID & pRecord->m_nMaskID) != (pRecord->m_nID & pRecord->m_nMaskID))) continue;
		if ((m_nFilterVendor != NO_FILTER_VALUE) && (m_nFilterVendor != pRecord->m_nVendorID)) continue;
		if ((m_strFilterTotalSize != NO_FILTER_STR) && (m_strFilterTotalSize != pRecord->GetStringSize())) continue;
		if ((m_nFilterPageSize != NO_FILTER_VALUE) && (m_nFilterPageSize != pRecord->m_nPageSize)) continue;
		if ((m_nFilterBus != NO_FILTER_VALUE) && (m_nFilterBus != pRecord->m_nBusWidth)) continue;
		if ((m_nFilterCS != NO_FILTER_VALUE) && (m_nFilterCS != pRecord->m_nCountCS)) continue;
		nInd = m_gridRecord.InsertItem(m_gridRecord.GetItemCount(), pRecord->m_strPartNumber);
		m_gridRecord.SetItemData(nInd, (DWORD_PTR)pRecord);
		m_gridRecord.SetItemText(nInd, 1, pRecord->GetStringSize());
		strCS.Format(_T("%d"), pRecord->m_nCountCS);
		m_gridRecord.SetItemText(nInd, 2, strCS);
		m_gridRecord.SetItemText(nInd, 3, CFlashRecord::GetStringBusWidth(pRecord->m_nBusWidth));
	}
	m_gridRecord.SortColumn(0, TRUE);
	m_gridRecord.SetCurSel(0);
	m_gridRecord.UnlockWindowUpdate();
	pRecord = m_gridRecord.GetItemCount() ? (CFlashRecord*)m_gridRecord.GetItemData(0) : NULL;
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_RECORD_CHANGED, (LPARAM)pRecord);
}
