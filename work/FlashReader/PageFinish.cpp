#include "stdafx.h"
#include "FlashReader.h"
#include "PageFinish.h"
#include "NewTaskDlg.h"

IMPLEMENT_DYNAMIC(CPageFinish, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageFinish, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_TASK, &CPageFinish::OnChangeEditTask)
END_MESSAGE_MAP()

CPageFinish::CPageFinish()
: CPropertyPage(CPageFinish::IDD)
{
}

CPageFinish::~CPageFinish()
{
}

void CPageFinish::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CPageFinish::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CRect			rectTemp;

	GetDlgItem(IDC_FINISH_INFO)->GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	m_dlgFlashInfo.Create(this, rectTemp.TopLeft());
	m_pSheet = (CNewTaskDlg*)GetParent();
	return TRUE;
}

BOOL CPageFinish::OnSetActive()
{
	m_bFlashEnable = m_pSheet->m_Flash.IsEnable();
	m_dlgFlashInfo.ShowFlashInfo(m_bFlashEnable ? &m_pSheet->m_Flash : NULL);
	SetButtons();
	return CPropertyPage::OnSetActive();
}

void CPageFinish::SetButtons()
{
	m_pSheet->SetWizardButtons(PSWIZB_BACK | ((m_bFlashEnable && !m_strTask.IsEmpty()) ? PSWIZB_FINISH : PSWIZB_DISABLEDFINISH));
}

BOOL CPageFinish::OnWizardFinish()
{
	CString			strDir = m_pSheet->m_strTaskName + m_strTask;
	if (PathIsDirectory(strDir))
	{
		if (IDNO == AfxMessageBox(IDS_TASK_EXISTS, MB_YESNO|MB_ICONEXCLAMATION)) return FALSE;
	}
	else
	{
		::CreateDirectory(strDir, NULL);
	}
	m_pSheet->m_strTaskName.Format(_T("%s\\%s"), strDir, m_strTask);
	return CPropertyPage::OnWizardFinish();
}

void CPageFinish::OnChangeEditTask()
{
	GetDlgItemText(IDC_EDIT_TASK, m_strTask);
	SetButtons();
}
