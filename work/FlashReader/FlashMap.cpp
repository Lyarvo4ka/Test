#include "StdAfx.h"
#include "FlashMap.h"

CFlashMap::CFlashMap()
: m_pMap(NULL)
{
	m_pMap = NULL;
}

CFlashMap::~CFlashMap()
{
	Clear();
}

void CFlashMap::Clear()
{
	if (m_pMap) delete m_pMap;
	m_pMap = NULL;
	m_nPageCount = 0;
	ZeroMemory(m_arrStateCount, sizeof(m_arrStateCount));
}

void CFlashMap::Create(ULONGLONG nPageCount)
{
	Clear();
	m_nPageCount = nPageCount;
	m_pMap = new BYTE[(UINT)m_nPageCount];
	SetAllPages(PageStateNo);
}

void CFlashMap::SetAllPages(BYTE nState)
{
	if (!m_pMap) return;
	memset(m_pMap, nState, (UINT)m_nPageCount);
	ZeroMemory(m_arrStateCount, sizeof(m_arrStateCount));
	m_arrStateCount[nState] = m_nPageCount;
}

void CFlashMap::SetPageState(ULONGLONG nGlobalPage, BYTE nState)
{
	CSingleLock	dataLock(&m_mutexMap);
	if (dataLock.Lock(500))
	{
		ChangeState(nGlobalPage, nState);
		dataLock.Unlock();
	}
	else
	{
		TRACE(_T("Fail lock SetPageState\n"));
	}
}

BYTE CFlashMap::GetPageState(ULONGLONG nGlobalPage)
{
	CSingleLock	dataLock(&m_mutexMap);
	BYTE		nRet = PageStateCount;
	if (dataLock.Lock(500))
	{
		if (m_pMap && (nGlobalPage < m_nPageCount)) nRet = m_pMap[nGlobalPage];
		dataLock.Unlock();
	}
	else
	{
		TRACE(_T("Fail lock GetPageState\n"));
	}
	return nRet;
}

void CFlashMap::GetArrStateCount(ULONGLONG* pArrStateCount)
{
	CSingleLock	dataLock(&m_mutexMap);
	if (dataLock.Lock(500))
	{
		memcpy(pArrStateCount, m_arrStateCount, sizeof(m_arrStateCount));
		dataLock.Unlock();
	}
	else
	{
		TRACE(_T("Fail lock GetArrStateCount\n"));
	}
}

void CFlashMap::ChangePageState(BYTE nStateOld, BYTE nStateNew)
{
	for (ULONG nPage = 0; nPage < m_nPageCount; nPage++)
	{
		if (m_pMap[nPage] == nStateOld) ChangeState(nPage, nStateNew);
	}
}

void CFlashMap::ChangePageState(UINT nPageFrom, UINT nPageTo, BYTE nStateNew)
{
	for (ULONG nPage = nPageFrom; nPage <= nPageTo; nPage++)
	{
		ChangeState(nPage, nStateNew);
	}
}

void CFlashMap::ChangeState(ULONGLONG nPage, BYTE nStateNew)
{
	if (nPage < m_nPageCount)
	{
		m_arrStateCount[m_pMap[nPage]]--;
		m_arrStateCount[nStateNew]++;
		m_pMap[nPage] = nStateNew;
	}
}

void CFlashMap::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// Storing code	
		ar << m_nPageCount;
		ar.Write(m_pMap, (UINT)m_nPageCount);
	}
	else
	{	// Loading code	
		Clear();
		ar >> m_nPageCount;
		m_pMap = new BYTE[(UINT)m_nPageCount];
		ar.Read(m_pMap, (UINT)m_nPageCount);
		for (ULONG nInd = 0; nInd < m_nPageCount; nInd++)
		{
			m_arrStateCount[m_pMap[nInd]]++;
		}
	}
}

