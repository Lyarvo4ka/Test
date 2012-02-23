#include "stdafx.h"
#include "FlashReader.h"
#include "PageStart.h"
#include "NewTaskDlg.h"

IMPLEMENT_DYNAMIC(CPageStart, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageStart, CPropertyPage)
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SELECT_CHECK, IDC_SELECT_CUSTOM, OnSelectChange)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnClickedBtnCheck)
	ON_FLASH_CHANGED(OnFlashChanged)
END_MESSAGE_MAP()

CPageStart::CPageStart(CFlashRecord& flashRecord)
: CPropertyPage(CPageStart::IDD)
, m_Flash(flashRecord)
, m_FlashParametersDlg(NULL, &m_Flash)
, m_FlashRecordsDlg(NULL, m_Flash.m_OwnerBase)
{
}

CPageStart::~CPageStart()
{
}

void CPageStart::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CPageStart::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CComboBox*		pCombo;
	CBitmap			bmp;
	CRect			rectWnd;
	CRect			rectTemp;

	m_pSheet = (CNewTaskDlg*)GetParent();
	m_ilState.Create(16, 16, ILC_COLORDDB|ILC_MASK, 0, 1);
	bmp.LoadBitmap(IDB_STATES);
	int i = m_ilState.Add(&bmp, RGB(255, 0, 255));
	bmp.DeleteObject();
	rectWnd.SetRectEmpty();
	::ImageList_GetIconSize(m_ilState, (int*)&rectWnd.right, (int*)&rectWnd.bottom);
	for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
	{
		GetDlgItem(IDC_LABLE_CE0 + nCS)->GetWindowRect(rectTemp);
		ScreenToClient(rectTemp);
		rectWnd.MoveToXY(rectTemp.right+1, rectTemp.top-1);
		m_pStateCS[nCS] = new CStatic;
		m_pStateCS[nCS]->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_ICON, rectWnd, this);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VCC);
	for (BYTE i = 0; i < CountPower; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringPower(i)), i);
	}
	SetSelection(pCombo, Power_1_8);
	GetDlgItem(IDC_LABEL_RECORDS)->GetWindowRect(rectWnd);
	ScreenToClient(rectWnd);
	m_FlashParametersDlg.Create(this, rectWnd.TopLeft());
	m_FlashRecordsDlg.Create(this, rectWnd.TopLeft());
	m_bCheckSuccess = FALSE;
	m_bConnected = m_pSheet->m_pOwner->IsConnected();
	m_nSelectID = m_bConnected ? IDC_SELECT_CHECK : IDC_SELECT_BASE;
	((CButton*)GetDlgItem(IDC_SELECT_CHECK))->EnableWindow(m_bConnected);
	((CButton*)GetDlgItem(m_nSelectID))->SetCheck(BST_CHECKED);
	m_bCheckSuccess = FALSE;
	m_nSignature = NO_FILTER_ID;
	SetCheckControls();
	return TRUE;
}

void CPageStart::OnDestroy()
{
	for (BYTE nCS = 0; nCS < MAX_CS; nCS++) if (m_pStateCS[nCS]) delete m_pStateCS[nCS];
	CPropertyPage::OnDestroy();
}

BOOL CPageStart::OnSetActive()
{
	SetNextButton();
	return CPropertyPage::OnSetActive();
}

LRESULT CPageStart::OnWizardNext()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnWizardNext();
}

void CPageStart::SetCheckControls()
{
	BOOL bEnable = (m_bConnected && (m_nSelectID == IDC_SELECT_CHECK));
	GetDlgItem(IDC_COMBO_VCC)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(bEnable);
	GetDlgItem(IDC_LABLE_ID)->EnableWindow(bEnable);
	GetDlgItem(IDC_LABLE_VENDOR)->EnableWindow(bEnable);
	GetDlgItem(IDC_LABLE_ONFI)->EnableWindow(bEnable);
	SetDlgItemText(IDC_LABLE_ID, _T(""));
	SetDlgItemText(IDC_LABLE_VENDOR, _T(""));
	SetDlgItemText(IDC_LABLE_ONFI, _T(""));
	SetDlgItemText(IDC_LABLE_BUS, _T(""));
	for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
	{
		m_pStateCS[nCS]->SetIcon(m_ilState.ExtractIcon(/*bEnable ? StateWait : */StateDisable));
	}
}

void CPageStart::SetNextButton()
{
	m_pSheet->SetWizardButtons(((m_nSelectID == IDC_SELECT_CHECK) && !m_bCheckSuccess) ? 0 : PSWIZB_NEXT);
}

void CPageStart::OnSelectChange(UINT nID)
{
	m_nSelectID = nID;
	switch (m_nSelectID)
	{
	case IDC_SELECT_CHECK:
		m_FlashRecordsDlg.SetFilterID(m_nSignature/*m_bCheckSuccess ? m_nSignature : NO_FILTER_ID*/);
		m_FlashRecordsDlg.ShowWindow(SW_SHOW);
		m_FlashParametersDlg.ShowWindow(SW_HIDE);
		break;
	case IDC_SELECT_BASE:
		m_FlashRecordsDlg.SetFilterID(NO_FILTER_ID);
		m_FlashRecordsDlg.ShowWindow(SW_SHOW);
		m_FlashParametersDlg.ShowWindow(SW_HIDE);
		break;
	case IDC_SELECT_CUSTOM:
		m_FlashRecordsDlg.ShowWindow(SW_HIDE);
		m_FlashParametersDlg.ShowWindow(SW_SHOW);
		break;
	};
	//m_FlashRecordsDlg.ShowWindow((nID == IDC_SELECT_CUSTOM) ? SW_HIDE : SW_SHOW);
	//m_FlashParametersDlg.ShowWindow((nID == IDC_SELECT_CUSTOM) ? SW_SHOW : SW_HIDE);
	//SetCheckControls();
	SetNextButton();
}

LRESULT CPageStart::OnFlashChanged(WPARAM wParam, LPARAM lParam)
{
	CFlashRecord*	pRecord = (CFlashRecord*)lParam;
	pRecord ? m_Flash.Copy(pRecord) : m_Flash.Clear();
	return 0;
}

void CPageStart::SetSelection(CComboBox* pCombo, DWORD_PTR nData)
{
	for (int i = 0; i < pCombo->GetCount(); i++)
	{
		if (nData == pCombo->GetItemData(i))
		{
			pCombo->SetCurSel(i);
			return;
		}
	}
	pCombo->SetCurSel(-1);
}

void CPageStart::OnClickedBtnCheck()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VCC);
	BYTE			nMaskCS;

	m_Flash.Clear();
	m_Flash.m_nPower = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	m_bCheckSuccess = m_pSheet->m_pOwner->CheckFlash(m_nSignature, nMaskCS, m_Flash, FALSE);
	for (int nCS = 0; nCS < MAX_CS; nCS++)
	{
		m_pStateCS[nCS]->SetIcon(m_ilState.ExtractIcon((nMaskCS & BIT(nCS)) ? StateComplete : StateFail/*StateWait*/));
	}
	SetDlgItemText(IDC_LABLE_ID, CFlashRecord::GetStringSignature(m_nSignature));
	SetDlgItemText(IDC_LABLE_VENDOR, m_pSheet->m_pOwner->GetStringVendor(m_nSignature));
	SetDlgItemText(IDC_LABLE_ONFI, CFlashRecord::GetStringSupportONFI(m_Flash.m_nONFI));
	SetDlgItemText(IDC_LABLE_BUS, CFlashRecord::GetStringBusWidth(m_Flash.m_nBusWidth));
	if (m_Flash.m_nONFI && !m_Flash.IsPresentInDatabase())
	{
		//
	}
	if (m_nSelectID == IDC_SELECT_CHECK) m_FlashRecordsDlg.SetFilterID(m_bCheckSuccess ? m_nSignature : NO_FILTER_ID);
	SetNextButton();
}
