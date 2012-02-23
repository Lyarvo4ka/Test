#include "stdafx.h"
#include "FlashReader.h"
#include "ChangePageStateDlg.h"
#include "FlashMap.h"

IMPLEMENT_DYNAMIC(CChangePageStateDlg, CDialog)

BEGIN_MESSAGE_MAP(CChangePageStateDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_GLOBAL, IDC_RADIO_GEOMETRY, &CChangePageStateDlg::OnChangeAdressingType)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ALL, IDC_RADIO_PAGE, &CChangePageStateDlg::OnChangeAdressingType)
END_MESSAGE_MAP()

CChangePageStateDlg::CChangePageStateDlg(ULONG nPageCount, ULONG nPagePerCS, ULONG nPagePerBlock, CWnd* pParent)
: CDialog(CChangePageStateDlg::IDD, pParent)
, m_nPageCount(nPageCount)
, m_nPagePerCS(nPagePerCS)
, m_nPagePerBlock(nPagePerCS)
{
	m_nGlobalPageFrom = 0;
	m_nGlobalPageTo = 0;
	m_nNewPageState = PageStateNo;
}

CChangePageStateDlg::~CChangePageStateDlg()
{
}

void CChangePageStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CChangePageStateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CButton*)GetDlgItem(IDC_RADIO_GLOBAL))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_PAGE))->SetCheck(BST_CHECKED);
	OnChangeAdressingType(IDC_RADIO_GLOBAL);
	OnChangeRangeType(IDC_RADIO_PAGE);
	return TRUE;
}

void CChangePageStateDlg::OnOK()
{
	// Check m_nGlobalPageFrom and m_nGlobalPageTo
	CDialog::OnOK();
}

void CChangePageStateDlg::OnChangeAdressingType(UINT nID)
{
	UINT nRangeType = GetCheckedRadioButton(IDC_RADIO_ALL, IDC_RADIO_PAGE);

	m_bGeometry = (nID == IDC_RADIO_GEOMETRY);
	GetDlgItem(IDC_RADIO_CS)->EnableWindow(m_bGeometry);
	GetDlgItem(IDC_RADIO_BLOCK)->EnableWindow(m_bGeometry);
	//if (!m_bGeometry && ((nRangeType == IDC_RADIO_CS) || (nRangeType == IDC_RADIO_CS))
	//{
	//	((CButton*)GetDlgItem(IDC_RADIO_PAGE))->SetCheck(BST_CHECKED);
	//	OnChangeRangeType(IDC_RADIO_PAGE);
	//}
}

void CChangePageStateDlg::OnChangeRangeType(UINT nID)
{
	ULONG	nCS;
	ULONG	nBlock;
	ULONG	nPage;

	if (nID == IDC_RADIO_ALL)
	{
		m_nGlobalPageFrom = 0;
		m_nGlobalPageTo = m_nPageCount - 1;
		GetDlgItem(IDC_EDIT_FROM_CS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TO_CS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FROM_BLOCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TO_BLOCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FROM_PAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TO_PAGE)->EnableWindow(FALSE);
	}
	else if (nID == IDC_RADIO_CS)
	{
		GlobalToGeometry(m_nGlobalPageFrom, nCS, nBlock, nPage);
		m_nGlobalPageFrom = GeometryToGlobal(nCS, 0, 0);
		m_nGlobalPageTo = m_nGlobalPageFrom + m_nPagePerCS - 1;
		GetDlgItem(IDC_EDIT_FROM_CS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TO_CS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FROM_BLOCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TO_BLOCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FROM_PAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TO_PAGE)->EnableWindow(FALSE);
	}
	else if (nID == IDC_RADIO_BLOCK)
	{
		GlobalToGeometry(m_nGlobalPageFrom, nCS, nBlock, nPage);
		m_nGlobalPageFrom = GeometryToGlobal(nCS, nBlock, 0);
		m_nGlobalPageTo = m_nGlobalPageFrom + m_nPagePerBlock - 1;
		GetDlgItem(IDC_EDIT_FROM_CS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TO_CS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FROM_BLOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TO_BLOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FROM_PAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TO_PAGE)->EnableWindow(FALSE);
	}
	else	//nID == IDC_RADIO_PAGE
	{
		GetDlgItem(IDC_EDIT_FROM_CS)->EnableWindow(m_bGeometry);
		GetDlgItem(IDC_EDIT_TO_CS)->EnableWindow(m_bGeometry);
		GetDlgItem(IDC_EDIT_FROM_BLOCK)->EnableWindow(m_bGeometry);
		GetDlgItem(IDC_EDIT_TO_BLOCK)->EnableWindow(m_bGeometry);
		GetDlgItem(IDC_EDIT_FROM_PAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TO_PAGE)->EnableWindow(TRUE);
	}
	UpdatePageInfo();
}

ULONG CChangePageStateDlg::GeometryToGlobal(ULONG nCS, ULONG nBlock, ULONG nPage)
{
	return nCS * m_nPagePerCS + nBlock * m_nPagePerBlock + nPage;
}

void CChangePageStateDlg::GlobalToGeometry(ULONG nGlobalPage, ULONG& nCS, ULONG& nBlock, ULONG& nPage)
{
	ULONG	nPageInCS = nGlobalPage % m_nPagePerCS;
	nCS = nGlobalPage / m_nPagePerCS;
	nBlock = nPageInCS / m_nPagePerBlock;
	nPage = nPageInCS % m_nPagePerBlock;
}

void CChangePageStateDlg::UpdatePageInfo()
{
	if (m_bGeometry)
	{
		ULONG	nCS;
		ULONG	nBlock;
		ULONG	nPage;
		GlobalToGeometry(m_nGlobalPageFrom, nCS, nBlock, nPage);
		SetDlgItemInt(IDC_EDIT_FROM_CS, nCS, FALSE);
		SetDlgItemInt(IDC_EDIT_FROM_BLOCK, nBlock, FALSE);
		SetDlgItemInt(IDC_EDIT_FROM_PAGE, nPage, FALSE);
		GlobalToGeometry(m_nGlobalPageTo, nCS, nBlock, nPage);
		SetDlgItemInt(IDC_EDIT_TO_CS, nCS, FALSE);
		SetDlgItemInt(IDC_EDIT_TO_BLOCK, nBlock, FALSE);
		SetDlgItemInt(IDC_EDIT_TO_PAGE, nPage, FALSE);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_FROM_CS, _T(""));
		SetDlgItemText(IDC_EDIT_FROM_BLOCK, _T(""));
		SetDlgItemInt(IDC_EDIT_FROM_PAGE, m_nGlobalPageFrom, FALSE);
		SetDlgItemText(IDC_EDIT_TO_CS, _T(""));
		SetDlgItemText(IDC_EDIT_TO_BLOCK, _T(""));
		SetDlgItemInt(IDC_EDIT_TO_PAGE, m_nGlobalPageTo, FALSE);
	}
}
