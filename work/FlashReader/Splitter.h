#pragma once

class CSplitter : public CSplitterWnd
{
public:
	BOOL	m_bCreated;

public:
	CSplitter();
	~CSplitter();
	CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual void StartTracking(int ht);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_DYNAMIC(CSplitter)
	DECLARE_MESSAGE_MAP()
};
