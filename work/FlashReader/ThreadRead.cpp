#include "StdAfx.h"
#include "ThreadRead.h"

#define MAX_BIT						8
#define LIMIT_TOTAL_BUFFERS_SIZE	(100 * 1048576)	// 100 MB

CThreadRead::CThreadRead(CFlashMap& flashMap)
: CBuffer::CAllocator(512, 10, 100)
, m_IOCP(0)
, m_FlashMap(flashMap)
, m_threadFile(m_IOCP, m_eventBreak, flashMap)
{
}

CThreadRead::~CThreadRead()
{
	Stop();
	WaitStopComplete();
}

BOOL CThreadRead::InitThread(HWND hOwner, CReader* pReader, CFlashRecord& flash, UINT nLimit, UINT nDelta, UINT nStepCount, BOOL bSeparateCS)
{
	BYTE		bufDummy[SECTOR_SIZE];
	ULONG		nReadedBytes;
	ULONG		nSize;
	WORD		nCommandParam;
	WORD		nSpeedParam;

	m_pReader = pReader;
	m_nMaxSectorCount = m_pReader->m_nMaxSectorCount;
	m_nPagePerCS = flash.GetPagePerCS();
	m_nPagePerLU = flash.GetPagePerLU();
	m_nFirstBitLU = flash.GetFirstBitLU();
	m_nCountLU = flash.m_nCountLU;
	m_nPageSize = flash.m_nPageSize;
	m_nCountLBA = 0;
	// Calculate m_nCountPage and m_nCountLBA
	m_nCountPage = 0;
	do
	{
		m_nCountPage++;
		nSize = m_nPageSize * m_nCountPage;
		m_nCountLBA = (WORD)(nSize / SECTOR_SIZE);
	}
	while (nSize % SECTOR_SIZE);
	
	if (!SetBufferSize(m_nCountLBA * SECTOR_SIZE)) return FALSE ;
	SetMaxUsedBuffers(LIMIT_TOTAL_BUFFERS_SIZE / GetBufferSize());
	m_nCorrectionCount = nStepCount;
	m_nBitLimit = (WORD)(m_nCorrectionCount * nLimit / 100);
	m_nBitDeltaUpper = (WORD)(m_nCorrectionCount * (nLimit + nDelta) / 100);
	m_nBitDeltaLower = (WORD)(m_nCorrectionCount * (nLimit - nDelta) / 100);

	nCommandParam = MAKE_COMMAND_PARAM(flash.m_nNumFullAdr, flash.m_nNumPageAdr, flash.m_nNumCmd);
	nSpeedParam = MAKE_SPEED_PARAM(flash.m_nSpeedAsyn, flash.m_nSpeedSyn, flash.m_nCmdType, flash.m_nReadType);
	m_nPowerParam = MAKE_POWER_PARAM(flash.m_nPowerIO, flash.m_nPowerCore, flash.m_nONFI, flash.m_nBusWidth);
	m_pReader->SetFlashParameters(flash.m_nPagePerBlock, m_nPageSize, m_nCountPage, nCommandParam, nSpeedParam, bufDummy, &nReadedBytes);
	m_threadFile.InitThread(hOwner, m_nPagePerCS, m_nCountPage, m_nPageSize, bSeparateCS);

	return TRUE;
}

void CThreadRead::Start(BYTE nReadMode, BYTE nCS, CString strDataPath)
{
	m_nCS = nCS;
	m_nReadMode = nReadMode;
	m_threadFile.Start(nReadMode, nCS, strDataPath);
	m_eventBreak.ResetEvent();
	StartThread();
}

void CThreadRead::Stop()
{
	m_eventBreak.SetEvent();
}

void CThreadRead::WaitStopComplete()
{
	m_threadFile.WaitThreadShutdown();
}

CBuffer* CThreadRead::ReadBuffer(DWORD nPageStart, WORD& nStopStatus)
{
	CBuffer*	pBuffer = AllocateBuffer();
	BYTE*		pReadBuf = (BYTE*)pBuffer->GetBuffer();
	WORD		nNeedSectors = m_nCountLBA;
	WORD		nSectorsToRead;
	ULONG		nReadedBytes;

	nStopStatus = StopOnComplete;
	do
	{
		nSectorsToRead = (nNeedSectors > m_nMaxSectorCount) ? m_nMaxSectorCount : nNeedSectors;
		if (!m_pReader->ReadPage(nPageStart, nSectorsToRead, pReadBuf, &nReadedBytes))
		{
			pBuffer->Release();
			pBuffer = NULL;
			nStopStatus = StopOnError;
			m_IOCP.PostStatus(NULL, MAKELONG(MessageThreadError, ErrorDeviceRead));
			break;
		}
		pReadBuf += nReadedBytes;
		nNeedSectors -= nSectorsToRead;
	} while (nNeedSectors);
	return pBuffer;
}

int CThreadRead::Run()
{
	CBuffer*	pBuf;
	WORD*		pBitArr;
	WORD*		pBitBuf;
	BYTE*		pByteBuf;
	BYTE		bufDummy[SECTOR_SIZE];
	ULONG		nReadedBytes;
	ULONGLONG	nPageGlobal;
	ULONGLONG	nPageEnd;
	ULONG		nPage;
	ULONG		nBitArraySize;
	ULONG		nPageIncrement;
	DWORD		nPowerState;
	WORD		nStopStatus;
	WORD		nCorrectionCnt;
	WORD		nPageInBuffer;
	BYTE		nStateForProccess;
	BYTE		nState;
	BYTE		nMask;
	BOOL		bIsModeCorrect;
	BOOL		bProccess;

	// Set Reader parameters and PowerOn
	m_pReader->SetReadParameters(MAKE_READ_PARAM(m_nCS, m_nReadMode), bufDummy, &nReadedBytes);
	m_pReader->PowerOn(m_nPowerParam, bufDummy, &nReadedBytes);
	nPowerState = *(DWORD*)bufDummy;
	if (nPowerState)
	{
		// PowerOff
		m_pReader->PowerOff(bufDummy, &nReadedBytes);
		m_IOCP.PostStatus(NULL, MAKELONG(MessageThreadError, ErrorPower));
		m_IOCP.PostStatus(NULL, MAKELONG(MessageThreadStop, StopOnError));
		return 0;
	}
	if (m_nReadMode == ModeCorrect)
	{
		bIsModeCorrect = TRUE;
		nStateForProccess = PageStateVerErr;
		nBitArraySize = MAX_BIT * m_nPageSize;
		pBitArr = new WORD[nBitArraySize];
	}
	else
	{
		bIsModeCorrect = FALSE;
		nStateForProccess = (m_nReadMode == ModeFirstRead) ? PageStateNo : PageStateOnce;
		pBitArr = NULL;
		nState = PageStateNo;
	}
	nPageGlobal = m_nCS * m_nPagePerCS;
	m_IOCP.PostStatus(NULL, MessageThreadStart);
	for (BYTE nLU = 0; nLU < m_nCountLU; nLU++)
	{
		nPage = m_nFirstBitLU * nLU;
		nPageEnd = nPage + m_nPagePerLU;
		do
		{
			if (m_eventBreak.WaitEvent(0))
			{
				nStopStatus = StopOnBreak;
				break;	//do while (nPage
			}
			nPageIncrement = bIsModeCorrect ? 1 : (ULONG)(((nPage + m_nCountPage) > nPageEnd) ? (nPageEnd - nPage) : m_nCountPage);
			bProccess = FALSE;
			for (ULONG nPageCheck = 0; nPageCheck < nPageIncrement; nPageCheck++)
			{
				if (nStateForProccess == m_FlashMap.GetPageState(nPageGlobal + nPageCheck))
				{
					bProccess = TRUE;
					break;	//for (nPageCheck
				}
			}
			if (bProccess)
			{
				if (bIsModeCorrect)
				{
					nCorrectionCnt = 0;
					nPageInBuffer = 0;
					ZeroMemory(pBitArr, nBitArraySize * sizeof(WORD));
					do
					{
						if (!nPageInBuffer)
						{
							if (nCorrectionCnt) pBuf->Release();
							pBuf = ReadBuffer(nPage, nStopStatus);
							if (nStopStatus == StopOnError) break;	//do while (nCorrectionCnt
							nPageInBuffer = m_nCountPage;
							pByteBuf = (BYTE*)pBuf->GetBuffer();
						}
						pBitBuf = pBitArr;
						for (WORD nByte = 0; nByte < m_nPageSize; nByte++)
						{
							nMask = 0x01;
							for (BYTE nBit = 0; nBit < MAX_BIT; nBit++)
							{
								if (*pByteBuf & nMask) (*pBitBuf)++;
								nMask <<= 1;
								pBitBuf++;
							}
							pByteBuf++;
						}
						nPageInBuffer--;
					}
					while (++nCorrectionCnt < m_nCorrectionCount);
					if (nStopStatus == StopOnError) break;	//do while (nPage - ERROR
					pByteBuf = (BYTE*)pBuf->GetBuffer();
					ZeroMemory(pByteBuf, m_nPageSize);
					pBitBuf = pBitArr;
					nState = PageStateCorHigh;
					for (WORD nByte = 0; nByte < m_nPageSize; nByte++)
					{
						nMask = 0x01;
						for (BYTE nBit = 0; nBit < MAX_BIT; nBit++)
						{
							if (*pBitBuf > m_nBitLimit) *pByteBuf |= nMask;
							if ((*pBitBuf > m_nBitDeltaLower) && (*pBitBuf < m_nBitDeltaUpper))
							{
								//TRACE("Page %d, Byte %d, Bit %d, Value %d\n", nPage, nByte, nBit, *pBitBuf);
								nState = PageStateCorLow;
							}
							nMask <<= 1;
							pBitBuf++;
						}
						pByteBuf++;
					}
				}
				else
				{
					pBuf = ReadBuffer(nPage, nStopStatus);
					if (!pBuf) break;	//do while (nPage - ERROR
				}
				pBuf->SetOperation((ULONG)nPageGlobal);
				pBuf->SetUsed(nPageIncrement);
				m_IOCP.PostStatus((ULONG_PTR)pBuf, MAKELONG(MessageThreadStep, nState));
			}
			nPageGlobal += nPageIncrement;
			nPage += nPageIncrement;
		}
		while (nPage < nPageEnd);
		if (nStopStatus == StopOnError) break;
	}
	if (pBitArr) delete pBitArr;
	// PowerOff
	m_pReader->PowerOff(bufDummy, &nReadedBytes);
	m_IOCP.PostStatus(NULL, MAKELONG(MessageThreadStop, nStopStatus));
	return 0;
}
