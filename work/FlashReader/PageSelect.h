#pragma once

class CPageSelect : public CPropertyPage
{
public:
	enum { IDD = IDD_PAGE_SELECT };

protected:

public:
	CPageSelect();
	virtual ~CPageSelect();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

	DECLARE_DYNAMIC(CPageSelect)
	DECLARE_MESSAGE_MAP()
};
