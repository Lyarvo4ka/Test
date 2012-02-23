#include "stdafx.h"
#include "FlashReader.h"
#include "NewTaskDlg.h"
#include "FlashReaderDoc.h"
#include "RecordEditDlg.h"

IMPLEMENT_DYNAMIC(CNewTaskDlg, CDialog)

BEGIN_MESSAGE_MAP(CNewTaskDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_TASK, OnChangeEditTask)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SELECT_BASE, IDC_SELECT_CUSTOM, OnSelectChange)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnClickedBtnCheck)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnClickedBtnReset)
	ON_FLASH_CHANGED(OnFlashChanged)
END_MESSAGE_MAP()

CNewTaskDlg::CNewTaskDlg(CFlashReaderDoc* pOwner, CWnd* pParent)
: CDialog(CNewTaskDlg::IDD, pParent)
, m_pOwner(pOwner)
, m_Flash(pOwner->m_Database)
, m_FlashParametersDlg(NULL, &m_Flash)
, m_FlashRecordsDlg(NULL, m_Flash.m_OwnerBase)
{
}

CNewTaskDlg::~CNewTaskDlg()
{
}

void CNewTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CNewTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox*		pCombo;
	CBitmap			bmp;
	CRect			rectWnd;
	CRect			rectTemp;

	m_nSignature = 0;
	m_bConnected = m_pOwner->IsConnected();

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
	for (BYTE i = PowerIO_1_8; i < CountPowerIO; i++)
	{
		pCombo->SetItemData(pCombo->AddString(CFlashRecord::GetStringPowerIO(i)), i);
	}
	SetComboSelection(pCombo, PowerIO_1_8);
	GetDlgItem(IDC_LABEL_RECORDS)->GetWindowRect(rectWnd);
	ScreenToClient(rectWnd);
	m_FlashParametersDlg.Create(this, rectWnd.TopLeft(), FALSE);
	m_FlashRecordsDlg.Create(this, rectWnd.TopLeft());
	GetDlgItem(IDC_COMBO_VCC)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_BUTTON_CHECK)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_LABLE_ID)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_LABLE_VENDOR)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_LABLE_ONFI)->EnableWindow(m_bConnected);
	((CButton*)GetDlgItem(IDC_SELECT_BASE))->SetCheck(BST_CHECKED);
	OnSelectChange(IDC_SELECT_BASE);
	OnClickedBtnReset();
	SetButtonOK();
	return TRUE;
}

void CNewTaskDlg::OnDestroy()
{
	for (BYTE nCS = 0; nCS < MAX_CS; nCS++) if (m_pStateCS[nCS]) delete m_pStateCS[nCS];
	CDialog::OnDestroy();
}

void CNewTaskDlg::SetComboSelection(CComboBox* pCombo, DWORD_PTR nData)
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

void CNewTaskDlg::SetButtonOK()
{
	GetDlgItem(IDOK)->EnableWindow(m_Flash.IsEnable() && !m_strTask.IsEmpty());
}

void CNewTaskDlg::OnOK()
{
	CString			strDir = m_strTaskName + m_strTask;
	if (PathIsDirectory(strDir))
	{
		if (IDNO == AfxMessageBox(IDS_TASK_EXISTS, MB_YESNO|MB_ICONEXCLAMATION)) return;
	}
	else
	{
		::CreateDirectory(strDir, NULL);
	}
	m_strTaskName.Format(_T("%s\\%s"), strDir, m_strTask);
	CDialog::OnOK();
}

void CNewTaskDlg::OnCancel()
{
	// Do not REMOVE!!!!
}

void CNewTaskDlg::OnClose()
{
	CDialog::OnClose();
	CDialog::OnCancel();
}

void CNewTaskDlg::OnChangeEditTask()
{
	GetDlgItemText(IDC_EDIT_TASK, m_strTask);
	SetButtonOK();
}

void CNewTaskDlg::OnSelectChange(UINT nID)
{

	m_nSelectID = nID;
	if (m_nSelectID == IDC_SELECT_BASE)
	{
		m_FlashRecordsDlg.ShowWindow(SW_SHOW);
		m_FlashParametersDlg.ShowWindow(SW_HIDE);
	}
	else
	{
		m_FlashRecordsDlg.ShowWindow(SW_HIDE);
		m_FlashParametersDlg.ShowWindow(SW_SHOW);
	}
}

LRESULT CNewTaskDlg::OnFlashChanged(WPARAM wParam, LPARAM lParam)
{
	if (wParam == FLASH_RECORD_CHANGED)
	{
		CFlashRecord*	pRecord = (CFlashRecord*)lParam;
		pRecord ? m_Flash.Copy(pRecord) : m_Flash.Clear();
	}
	SetButtonOK();
	return 0;
}

void CNewTaskDlg::OnClickedBtnCheck()
{
	CComboBox*		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_VCC);
	BYTE			nMaskCS;

	m_Flash.Clear();
	m_Flash.m_nPowerIO = (BYTE)pCombo->GetItemData(pCombo->GetCurSel());
	m_pOwner->CheckNewFlash(m_nSignature, nMaskCS, m_Flash);
	for (int nCS = 0; nCS < MAX_CS; nCS++)
	{
		m_pStateCS[nCS]->SetIcon(m_ilState.ExtractIcon((nMaskCS & BIT(nCS)) ? StateComplete : StateFail));
	}
	SetDlgItemText(IDC_LABLE_ID, CFlashRecord::GetStringSignature(m_nSignature));
	SetDlgItemText(IDC_LABLE_VENDOR, m_Flash.GetStringVendor(m_nSignature));
	SetDlgItemText(IDC_LABLE_ONFI, CFlashRecord::GetStringSupportONFI(m_Flash.m_nONFI));
	SetDlgItemText(IDC_LABLE_BUS, CFlashRecord::GetStringBusWidth(m_Flash.m_nBusWidth));
	SetButtonOK();
	if (m_Flash.m_nONFI && !m_Flash.m_OwnerBase.GetRecord(m_Flash.m_strPartNumber))
	{
		if (IDYES == AfxMessageBox(IDS_CHIP_NOT_PRESENT, MB_ICONEXCLAMATION|MB_YESNO))
		{
			CFlashRecord* pRecord = m_FlashRecordsDlg.AddRecord(&m_Flash);
			if (pRecord) 
			{
				pRecord->m_OwnerBase.Save();
				m_Flash.Copy(pRecord);
			}
		}
	}
	if (m_nSelectID == IDC_SELECT_BASE)
	{
		m_FlashRecordsDlg.SetFilterCheck(&m_Flash);
	}
	else
	{
		m_FlashParametersDlg.ShowWindow(SW_HIDE);
		m_FlashParametersDlg.ShowWindow(SW_SHOW);
	}
}

void CNewTaskDlg::OnClickedBtnReset()
{
	SetDlgItemText(IDC_LABLE_ID, _T(""));
	SetDlgItemText(IDC_LABLE_VENDOR, _T(""));
	SetDlgItemText(IDC_LABLE_ONFI, _T(""));
	SetDlgItemText(IDC_LABLE_BUS, _T(""));
	for (BYTE nCS = 0; nCS < MAX_CS; nCS++)
	{
		m_pStateCS[nCS]->SetIcon(m_ilState.ExtractIcon(StateDisable));
	}
	m_FlashRecordsDlg.SetFilterCheck(NULL);
}
