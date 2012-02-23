#pragma once

enum PageState
{
	PageStateNo,		// не читалась				(серый)
	PageStateOnce,		// прочитана однократно		(желтый)
	PageStateVerErr,	// верификация ERR			(красный)
	PageStateVerOk,		// верификация ОК			(светло-зеленый)
	PageStateCorHigh,	// откорректирована	HIGH	(темно-зеленый)
	PageStateCorLow,	// откорректирована	LOW		(синий)
	PageStateCount
};

class CFlashMap
{
public:

private:
	CMutex		m_mutexMap;
	ULONGLONG	m_arrStateCount[PageStateCount];
	ULONGLONG	m_nPageCount;
	BYTE*		m_pMap;

public:
	CFlashMap();
	~CFlashMap();
	void Clear();
	void Create(ULONGLONG nPageCount);
	void SetAllPages(BYTE nState);
	void SetPageState(ULONGLONG nGlobalPage, BYTE nState);
	BYTE GetPageState(ULONGLONG nGlobalPage);
	void GetArrStateCount(ULONGLONG* pArrStateCount);
	void ChangePageState(BYTE nStateOld, BYTE nStateNew);
	void ChangePageState(UINT nPageFrom, UINT nPageTo, BYTE nStateNew);
	void Serialize(CArchive& ar);

private:
	void ChangeState(ULONGLONG nPage, BYTE nStateNew);
};
