#include "stdafx.h"
#include "FlashReader.h"
#include "SelectReaderDlg.h"

IMPLEMENT_DYNAMIC(CSelectReaderDlg, CDialog)

BEGIN_MESSAGE_MAP(CSelectReaderDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICES, &CSelectReaderDlg::OnDblClkTreeDevices)
END_MESSAGE_MAP()

CSelectReaderDlg::CSelectReaderDlg(CWnd* pParent)
: CDialog(CSelectReaderDlg::IDD, pParent)
{
	CBitmap			bmp;

	m_ilDevice.Create(32, 32, ILC_COLORDDB|ILC_MASK, 0, 1);
	bmp.LoadBitmap(IDB_DEVICES);
	m_ilDevice.Add(&bmp, RGB(255, 0, 255));
	bmp.DeleteObject();
}

CSelectReaderDlg::~CSelectReaderDlg()
{
}

void CSelectReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICES, m_treeDevices);
}

BOOL CSelectReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CReader*	pReader;
	CString		strTitle;
	HTREEITEM	hCurrentItem;

	m_treeDevices.SetImageList(&m_ilDevice, TVSIL_NORMAL);
	m_treeDevices.SetBkColor(GetSysColor(COLOR_3DFACE));
	for (POSITION pos = m_pListReader->GetHeadPosition(); pos != NULL; )
	{
		pReader = m_pListReader->GetNext(pos);
		strTitle.Format(_T("%s, %s"), pReader->GetModel(), pReader->GetSerialNumber());
		hCurrentItem = m_treeDevices.InsertItem(strTitle, pReader->m_nState, pReader->m_nState);
		m_treeDevices.SetItemData(hCurrentItem, (DWORD_PTR)pReader);
	}
	hCurrentItem = m_treeDevices.GetFirstVisibleItem();
	if (hCurrentItem != NULL) m_treeDevices.SelectItem(hCurrentItem);
	m_treeDevices.SetFocus();
	return TRUE;
}

void CSelectReaderDlg::OnDblClkTreeDevices(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	OnOK();
}

void CSelectReaderDlg::OnOK()
{
	HTREEITEM		hCurrentItem = m_treeDevices.GetSelectedItem();
	if (hCurrentItem == NULL)
	{
		 AfxMessageBox(IDS_WRONG_READER, MB_OK|MB_ICONEXCLAMATION);
		 return;
	}
	m_pReader = (CReader*)m_treeDevices.GetItemData(hCurrentItem);
	if (m_pReader->m_nState == ReaderBusy)
	{
		 AfxMessageBox(IDS_READER_BUSY, MB_OK|MB_ICONEXCLAMATION);
		 return;
	}
	if (m_pReader->m_nState == ReaderBad)
	{
		 AfxMessageBox(IDS_READER_BAD, MB_OK|MB_ICONEXCLAMATION);
		 return;
	}
	CDialog::OnOK();
}
