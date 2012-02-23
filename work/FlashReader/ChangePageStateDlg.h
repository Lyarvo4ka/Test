#pragma once

class CChangePageStateDlg : public CDialog
{
public:
	enum { IDD = IDD_CHANGE_PAGE_STATE };
	ULONG			m_nGlobalPageFrom;
	ULONG			m_nGlobalPageTo;
	BYTE			m_nNewPageState;

protected:
	ULONG			m_nCsCount;
	ULONG			m_nPageCount;
	ULONG			m_nPagePerCS;
	ULONG			m_nPagePerBlock;
	BOOL			m_bGeometry;

public:
	CChangePageStateDlg(ULONG nPageCount, ULONG nPagePerCS, ULONG nPagePerBlock, CWnd* pParent = NULL);
	virtual ~CChangePageStateDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeAdressingType(UINT nID);
	afx_msg void OnChangeRangeType(UINT nID);

private:
	ULONG GeometryToGlobal(ULONG nCS, ULONG nBlock, ULONG nPage);
	void GlobalToGeometry(ULONG nGlobalPage, ULONG& nCS, ULONG& nBlock, ULONG& nPage);
	void UpdatePageInfo();

	DECLARE_DYNAMIC(CChangePageStateDlg)
	DECLARE_MESSAGE_MAP()
};
