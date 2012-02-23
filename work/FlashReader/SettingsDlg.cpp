#include "stdafx.h"
#include "FlashReader.h"
#include "SettingsDlg.h"

#define STEP_COUNT_MIN		5
#define LIMIT_MIN			5
#define LIMIT_MAX			95

IMPLEMENT_DYNAMIC(CBrowseDirectory, COXEdit)

BEGIN_MESSAGE_MAP(CBrowseDirectory, COXEdit)
END_MESSAGE_MAP()

CBrowseDirectory::CBrowseDirectory()
{
}

CBrowseDirectory::~CBrowseDirectory()
{
}

void CBrowseDirectory::OnDropButton()
{
	WCHAR			szPathStr[MAX_PATH] = { 0 };
	WCHAR			szPath[MAX_PATH];
	BROWSEINFO		bi;
	LPITEMIDLIST	lpilDirectory;
	LPITEMIDLIST	lpilRoot;
    
	SHGetSpecialFolderLocation(m_hWndOwner, CSIDL_DRIVES/*CSIDL_DESKTOP*/, &lpilRoot);
	if(lpilRoot != NULL)
	{
		CString	strTitle;
		strTitle.LoadString(IDS_SELECT_DIRECTORY);
		bi.hwndOwner = m_hWndOwner;
		bi.pidlRoot = lpilRoot;
		bi.pszDisplayName = szPath;
		bi.lpszTitle = strTitle.GetBuffer();
		bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
		bi.lpfn = NULL;
		bi.lParam = 0L;
		bi.iImage = 0;
		lpilDirectory = SHBrowseForFolder(&bi);
		strTitle.ReleaseBuffer();
		GlobalFree(GlobalHandle(lpilRoot));
		if(lpilDirectory != NULL)
		{
			if (SHGetPathFromIDList(lpilDirectory, szPathStr)) SetWindowText(szPathStr);
			GlobalFree(GlobalHandle(lpilDirectory));
		}
	}
}

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_DEFAULTS, &CSettingsDlg::OnBtnDefaults)
	ON_BN_CLICKED(IDC_CHECK_LOG_ENABLE, &CSettingsDlg::OnClickedCheckLogEnable)
END_MESSAGE_MAP()

CSettingsDlg::CSettingsDlg(CWnd* pParent)
: CDialog(CSettingsDlg::IDD, pParent)
{
	m_nLimit = LIMIT_DEFAULT;
	m_nDelta = DELTA_DEFAULT;
	m_nStepCount = STEP_COUNT_DEFAULT;
	m_nLogSize = 1;
	m_bLogEnable = FALSE;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LIMIT, m_sldLimit);
	DDX_Control(pDX, IDC_SLIDER_DELTA, m_sldDelta);
	DDX_Control(pDX, IDC_EDIT_DIRECTORY, m_editDirectory);
}

BOOL CSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_editDirectory.SetWindowText(m_strDirectory);
	((CEdit*)GetDlgItem(IDC_EDIT_LOG_SIZE))->SetLimitText(4);
	SetDlgItemInt(IDC_EDIT_LOG_SIZE, m_nLogSize);
	((CButton*)GetDlgItem(IDC_CHECK_SEPARATE_CS))->SetCheck(m_bSeparateCS ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_LOG_ENABLE))->SetCheck(m_bLogEnable ? BST_CHECKED : BST_UNCHECKED);
	OnClickedCheckLogEnable();
	((CEdit*)GetDlgItem(IDC_STEP_COUNT))->SetLimitText(4);
	SetAll();
	return TRUE;
}


void CSettingsDlg::OnOK()
{
	m_editDirectory.GetWindowText(m_strDirectory);
	m_bSeparateCS = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_SEPARATE_CS))->GetCheck());
	m_nLogSize = GetDlgItemInt(IDC_EDIT_LOG_SIZE, 0, FALSE);
	m_nStepCount = GetDlgItemInt(IDC_STEP_COUNT, 0, FALSE);
	if (m_nStepCount < STEP_COUNT_MIN)
	{
		CString	strMessage;
		strMessage.Format(IDS_WRONG_STEP_COUNT, STEP_COUNT_MIN);
		AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	CDialog::OnOK();
}

void CSettingsDlg::OnBtnDefaults()
{
	m_nLimit = LIMIT_DEFAULT;
	m_nDelta = DELTA_DEFAULT;
	m_nStepCount = STEP_COUNT_DEFAULT;
	SetAll();
}

void CSettingsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	if (nSBCode == SB_ENDSCROLL) return;
	if (pScrollBar == (CScrollBar*)&m_sldLimit)
	{
		m_nLimit = m_sldLimit.GetPos();
		ShowValue(m_sldLimit, IDC_LIMIT_VALUE, m_nLimit);
		UpdateDeltaRange();
	}
	if (pScrollBar == (CScrollBar*)&m_sldDelta)
	{
		m_nDelta = m_sldDelta.GetPos();
		ShowValue(m_sldDelta, IDC_DELTA_VALUE, m_nDelta);
		UpdateLimitRange();
	}
}

void CSettingsDlg::SetAll()
{
	m_sldLimit.SetPos(m_nLimit);
	m_sldDelta.SetPos(m_nDelta);
	UpdateLimitRange();
	UpdateDeltaRange();
	SetDlgItemInt(IDC_STEP_COUNT, m_nStepCount, FALSE);
}

void CSettingsDlg::ShowValue(CSliderCtrl& sldCtrl, UINT nValueID, INT nValue)
{
	CRect	rectSlider;
	CRect	rectThump;
	CRect	rectValue;
	CWnd*	pWnd = GetDlgItem(nValueID);
	sldCtrl.GetWindowRect(rectSlider);
	ScreenToClient(rectSlider);
	pWnd->GetWindowRect(rectValue);
	sldCtrl.GetThumbRect(rectThump);
	rectValue.MoveToXY(rectSlider.left + rectThump.left - (rectValue.Width() - rectThump.Width()) / 2, rectSlider.top - rectValue.Height());
	pWnd->MoveWindow(rectValue);
	SetDlgItemInt(nValueID, nValue, FALSE);
}

void CSettingsDlg::UpdateLimitRange()
{
	m_sldLimit.SetRange(__max(LIMIT_MIN, m_nDelta), __min(LIMIT_MAX, 100 - m_nDelta));
	m_sldLimit.SetTicFreq(5);
	ShowValue(m_sldLimit, IDC_LIMIT_VALUE, m_nLimit);
}

void CSettingsDlg::UpdateDeltaRange()
{
	m_sldDelta.SetRange(0, __min(m_nLimit - LIMIT_MIN, LIMIT_MAX - m_nLimit));
	m_sldDelta.SetTicFreq(5);
	ShowValue(m_sldDelta, IDC_DELTA_VALUE, m_nDelta);
}

void CSettingsDlg::OnClickedCheckLogEnable()
{
	m_bLogEnable = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_LOG_ENABLE))->GetCheck());
	GetDlgItem(IDC_EDIT_LOG_SIZE)->EnableWindow(m_bLogEnable);
}
