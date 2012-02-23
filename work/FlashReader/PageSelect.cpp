#include "stdafx.h"
#include "FlashReader.h"
#include "PageSelect.h"

IMPLEMENT_DYNAMIC(CPageSelect, CPropertyPage)

BEGIN_MESSAGE_MAP(CPageSelect, CPropertyPage)
END_MESSAGE_MAP()

CPageSelect::CPageSelect()
: CPropertyPage(CPageSelect::IDD)
{
}

CPageSelect::~CPageSelect()
{
}

void CPageSelect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CPageSelect::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CRect	rectWnd;
	GetWindowRect(&rectWnd);
	rectWnd.MoveToXY(0,0);
	return TRUE;
}

BOOL CPageSelect::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	pSheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}
