#include "stdafx.h"
#include "FlashReader.h"
#include "FlashParametersDlg.h"

IMPLEMENT_DYNAMIC(CFlashParametersDlg, CDialog)

BEGIN_MESSAGE_MAP(CFlashParametersDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_VENDOR, &CFlashParametersDlg::OnComboVendor)
	ON_CBN_SELCHANGE(IDC_COMBO_CS_CHIP, &CFlashParametersDlg::OnComboCsChip)
	ON_CBN_SELCHANGE(IDC_COMBO_BUS, &CFlashParametersDlg::OnComboBus)
	ON_CBN_SELCHANGE(IDC_COMBO_CMD_TYPE, &CFlashParametersDlg::OnComboCmdType)
	ON_CBN_SELCHANGE(IDC_COMBO_CMD_CYCLE, &CFlashParametersDlg::OnComboCmdCycle)
	ON_CBN_SELCHANGE(IDC_COMBO_FULL_ADR_CYCLE, &CFlashParametersDlg::OnComboFullAdrCycle)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE_ADR_CYCLE, &CFlashParametersDlg::OnCombPageAdrCycle)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CFlashParametersDlg::OnComboSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_POWER_IO, &CFlashParametersDlg::OnComboPowerIO)
	ON_CBN_SELCHANGE(IDC_COMBO_ONFI, &CFlashParametersDlg::OnComboONFI)
	ON_EN_CHANGE(IDC_EDIT_PAGE_SIZE, &CFlashParametersDlg::OnChangePageSize)
	ON_EN_CHANGE(IDC_EDIT_PAGE_BLOCK, &CFlashParametersDlg::OnChangePageBlock)
	ON_EN_CHANGE(IDC_EDIT_BLOCK_LU, &CFlashParametersDlg::OnChangeBlockLU)
	ON_EN_CHANGE(IDC_EDIT_LU_CS, &CFlashParametersDlg::OnChangeCountLU)
	ON_EN_CHANGE(IDC_EDIT_PARTNUMBER, &CFlashParametersDlg::OnChangePartnumber)
	ON_EN_CHANGE(IDC_EDIT_ID, &CFlashParametersDlg::OnChangeID)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_MASK_0, IDC_MASK_7, &CFlashParametersDlg::OnChangeMask)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

CFlashParametersDlg::CFlashParametersDlg(CWnd* pParent, CFlashRecord* pFlash)
: CDialog(CFlashParametersDlg::IDD, pParent)
, m_pFlash(pFlash)
{
	m_pEditID = NULL;
}

CFlashParametersDlg::~CFlashParametersDlg()
{
}

void CFlashParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CFlashParametersDlg::Create(CWnd* pParentWnd, CPoint ptWnd, BOOL bNew)
{
	if (!CDialog::Create(IDD, pParentWnd)) return FALSE;
	CFlashDatabase&	database(m_pFlash->m_OwnerBase);
	CComboBox*		pCombo;
	CRect			rectParameters;
	CVendorRecord*	pVendor;
	INT				i;
	LOGFONT			lf;

	m_bNew = bNew;
	// Move window
	GetWindowRect(&rectParameters);
	rectParameters.MoveToXY(ptWnd);
	MoveWindow(&rectParameters);
	// Init controls
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VENDOR);
	for (POSITION nPosVendor = database.GetVendorHeadPosition(); nPosVendor != NULL; )
	{
		pVendor =  database.GetVendorNext(nPosVendor);
		pCombo->SetItemData(pCombo->AddString(pVendor->m_strVendor), pVendor->m_nID);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CS_CHIP);
	pCombo->SetItemData(pCombo->AddString(_T(" 1")), 0x01);
	pCombo->SetItemData(pCombo->AddString(_T(" 2")), 0x02);
	pCombo->SetItemData(pCombo->AddString(_T(" 3")), 0x03);
	pCombo->SetItemData(pCombo->AddString(_T(" 4")), 0x04);
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CMD_CYCLE);
	pCombo->SetItemData(pCombo->AddString(_T(" 1")), 0x01);
	pCombo->SetItemData(pCombo->AddString(_T(" 2")), 0x02);
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FULL_ADR_CYCLE);
	pCombo->SetItemData(pCombo->AddString(_T(" 3")), 0x03);
	pCombo->SetItemData(pCombo->AddString(_T(" 4")), 0x04);
	pCombo->SetItemData(pCombo->AddString(_T(" 5")), 0x05);
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PAGE_ADR_CYCLE);
	pCombo->SetItemData(pCombo->AddString(_T(" 1")), 0x01);
	pCombo->SetItemData(pCombo->AddString(_T(" 2")), 0x02);
	pCombo->SetItemData(pCombo->AddString(_T(" 3")), 0x03);
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CMD_TYPE);
	for (i = 0; i < CountCommandType; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringCommandType(i)), i);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BUS);
	for (i = 0; i < CountBusWidth; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringBusWidth(i)), i);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	for (i = 0; i < CountSpeedAsyn; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringSpeed(i)), i);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_POWER_IO);
	for (i = 0; i < CountPowerIO; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringPowerIO(i)), i);
	}
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ONFI);
	for (i = 0; i < CountSupportONFI; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringSupportONFI(i)), i);
	}
	((CEdit*)GetDlgItem(IDC_EDIT_LU_CS))->SetLimitText(3);
	((CEdit*)GetDlgItem(IDC_EDIT_BLOCK_LU))->SetLimitText(5);
	((CEdit*)GetDlgItem(IDC_EDIT_PAGE_BLOCK))->SetLimitText(5);
	((CEdit*)GetDlgItem(IDC_EDIT_PAGE_SIZE))->SetLimitText(5);
	m_pEditID = DYNAMIC_DOWNCAST(CDumpEdit, GetDlgItem(IDC_EDIT_ID));
	ASSERT(m_pEditID != NULL);
	m_pEditID->GetFont(&lf);
	lf.lfHeight = 14;
	lf.lfWeight = FW_MEDIUM;
	m_pEditID->SetFont(&lf, false);
	m_pEditID->SetShowAddress(false, false);
	m_pEditID->SetShowAscii(false, false);
	m_pEditID->SetBytesPerRow(COUNT_ID_MASK, false, false);
	m_pEditID->SetDirectDataPtr((BYTE*)&m_pFlash->m_nID, COUNT_ID_MASK, false);
	// Show window
	ShowWindow(SW_SHOW);
	return TRUE;
}

void CFlashParametersDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		CButton*	pBtn;
		BYTE*		pMask = (BYTE*)&m_pFlash->m_nMaskID;
		BOOL		bCustom = (!m_pFlash->IsEnable() && !m_bNew);

		if (bCustom)
		{
			m_pFlash->m_strPartNumber = _T("Custom chip");
		}
		else
		{
			if (!m_pFlash->m_nVendorID) m_pFlash->m_nVendorID = (BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_VENDOR))->GetItemData(0);
		}
		GetDlgItem(IDC_COMBO_VENDOR)->EnableWindow(m_bNew);
		SetDlgItemText(IDC_EDIT_PARTNUMBER, m_pFlash->m_strPartNumber);
		GetDlgItem(IDC_EDIT_PARTNUMBER)->EnableWindow(m_bNew);
		for (UINT nID = 0; nID < COUNT_ID_MASK; nID++)
		{
			pBtn = (CButton*)GetDlgItem(IDC_MASK_0 + nID);
			pBtn->SetCheck(pMask[nID] ? BST_CHECKED : BST_UNCHECKED);
			pBtn->EnableWindow((nID < 2) ? FALSE : !bCustom);
		}
		m_pEditID->EnableWindow(!bCustom);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_VENDOR), m_pFlash->m_nVendorID);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_CS_CHIP), m_pFlash->m_nCountCS);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_CMD_TYPE), m_pFlash->m_nCmdType);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_CMD_CYCLE), m_pFlash->m_nNumCmd);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_FULL_ADR_CYCLE), m_pFlash->m_nNumFullAdr);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_PAGE_ADR_CYCLE), m_pFlash->m_nNumPageAdr);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_BUS), m_pFlash->m_nBusWidth);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_SPEED), m_pFlash->m_nSpeedAsyn);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_POWER_IO), m_pFlash->m_nPowerIO);
		SetSelection((CComboBox*)GetDlgItem(IDC_COMBO_ONFI), m_pFlash->m_nONFI);
		SetDlgItemInt(IDC_EDIT_LU_CS, m_pFlash->m_nCountLU, FALSE);
		SetDlgItemInt(IDC_EDIT_BLOCK_LU, m_pFlash->m_nBlockPerLU, FALSE);
		SetDlgItemInt(IDC_EDIT_PAGE_BLOCK, m_pFlash->m_nPagePerBlock, FALSE);
		SetDlgItemInt(IDC_EDIT_PAGE_SIZE, m_pFlash->m_nPageSize, FALSE);
	}
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
	CDialog::OnShowWindow(bShow, nStatus);
}

void CFlashParametersDlg::OnOK()
{
	// Do not REMOVE!!!!
}

void CFlashParametersDlg::OnCancel()
{
	// Do not REMOVE!!!!
}

void CFlashParametersDlg::SetSelection(CComboBox* pCombo, DWORD_PTR nData)
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

void CFlashParametersDlg::UpdateSize()
{
	GetDlgItem(IDC_FLASH_TOTAL_SIZE)->SetWindowText(m_pFlash->GetStringSize());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboVendor()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VENDOR);
	m_pFlash->m_nVendorID = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboCsChip()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CS_CHIP);
	m_pFlash->m_nCountCS = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	UpdateSize();
}

void CFlashParametersDlg::OnComboBus()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BUS);
	m_pFlash->m_nBusWidth = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboCmdType()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CMD_TYPE);
	m_pFlash->m_nCmdType = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboCmdCycle()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CMD_CYCLE);
	m_pFlash->m_nNumCmd = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboFullAdrCycle()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FULL_ADR_CYCLE);
	m_pFlash->m_nNumFullAdr = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnCombPageAdrCycle()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PAGE_ADR_CYCLE);
	m_pFlash->m_nNumPageAdr = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboSpeed()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SPEED);
	m_pFlash->m_nSpeedAsyn = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboPowerIO()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_POWER_IO);
	m_pFlash->m_nPowerIO = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnComboONFI()
{
	CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ONFI);
	m_pFlash->m_nONFI = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnChangePartnumber()
{
	GetDlgItemText(IDC_EDIT_PARTNUMBER, m_pFlash->m_strPartNumber);
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnChangePageSize()
{
	UINT	nValue = GetDlgItemInt(IDC_EDIT_PAGE_SIZE, FALSE);
	if (nValue < 65536)
	{
		m_pFlash->m_nPageSize = (WORD)nValue;
	}
	else
	{
		m_pFlash->m_nPageSize = 65535;
		SetDlgItemInt(IDC_EDIT_PAGE_SIZE, m_pFlash->m_nPageSize, FALSE);
	}
	UpdateSize();
}

void CFlashParametersDlg::OnChangePageBlock()
{
	UINT	nValue = GetDlgItemInt(IDC_EDIT_PAGE_BLOCK, FALSE);
	if (nValue < 65536)
	{
		m_pFlash->m_nPagePerBlock = (WORD)nValue;
	}
	else
	{
		m_pFlash->m_nPagePerBlock = 65535;
		SetDlgItemInt(IDC_EDIT_PAGE_BLOCK, m_pFlash->m_nPagePerBlock, FALSE);
	}
	UpdateSize();
}

void CFlashParametersDlg::OnChangeBlockLU()
{
	UINT	nValue = GetDlgItemInt(IDC_EDIT_BLOCK_LU, FALSE);
	if (nValue < 65536)
	{
		m_pFlash->m_nBlockPerLU = (WORD)nValue;
	}
	else
	{
		m_pFlash->m_nBlockPerLU = 65535;
		SetDlgItemInt(IDC_EDIT_BLOCK_LU, m_pFlash->m_nBlockPerLU, FALSE);
	}
	UpdateSize();
}

void CFlashParametersDlg::OnChangeCountLU()
{
	UINT	nValue = GetDlgItemInt(IDC_EDIT_LU_CS, FALSE);
	if (nValue < 255)
	{
		m_pFlash->m_nCountLU = (BYTE)nValue;
	}
	else
	{
		m_pFlash->m_nCountLU = 255;
		SetDlgItemInt(IDC_EDIT_LU_CS, m_pFlash->m_nCountLU, FALSE);
	}
	UpdateSize();
}

void CFlashParametersDlg::OnChangeID()
{
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}

void CFlashParametersDlg::OnChangeMask(UINT nID)
{
	CButton*	pBtn = (CButton*)GetDlgItem(nID);
	BYTE*		pMask = (BYTE*)&m_pFlash->m_nMaskID;
	pMask[nID - IDC_MASK_0] = (pBtn->GetCheck() == BST_CHECKED) ? 0xFF : 0x00;
	GetParent()->SendMessage(WM_FLASH_CHANGED, FLASH_PARAMETERS_CHANGED, 0);
}
