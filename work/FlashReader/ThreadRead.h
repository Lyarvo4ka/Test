#pragma once

#include "ThreadFile.h"
#include "FlashRecord.h"

class CThreadRead : public CThreadExt, public CBuffer::CAllocator
{
public:

private:
	CThreadFile			m_threadFile;
	CIOCompletionPort	m_IOCP;
	CManualResetEvent	m_eventBreak;
	CFlashMap&			m_FlashMap;
	CReader*			m_pReader;
	ULONGLONG			m_nPagePerCS;
	ULONGLONG			m_nPagePerLU;
	ULONG				m_nFirstBitLU;
	WORD				m_nMaxSectorCount;
	WORD				m_nCountLU;
	WORD				m_nCountLBA;
	WORD				m_nCountPage;
	WORD				m_nPageSize;
	WORD				m_nCorrectionCount;;
	WORD				m_nBitLimit;
	WORD				m_nBitDeltaUpper;
	WORD				m_nBitDeltaLower;
	WORD				m_nPowerParam;
	BYTE				m_nReadMode;
	BYTE				m_nCS;

public:
	CThreadRead(CFlashMap& flashMap);
	~CThreadRead();
	BOOL InitThread(HWND hOwner, CReader* pReader, CFlashRecord& flash, UINT nLimit, UINT nDelta, UINT nStepCount, BOOL bSeparateCS);
	void Start(BYTE nReadMode, BYTE nCS, CString strDataPath);
	void Stop();
	WORD GetReadPageCount() { return m_nCountPage; };
	void WaitStopComplete();

protected:

private:
	virtual int Run();
	CBuffer* ReadBuffer(DWORD nPageStart, WORD& nStopStatus);
};
