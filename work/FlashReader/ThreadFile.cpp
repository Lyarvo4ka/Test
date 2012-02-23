#include "StdAfx.h"
#include "ThreadFile.h"
#include "Reader.h"

CThreadFile::CThreadFile(CIOCompletionPort& conpletitionPort, CManualResetEvent& eventBreak, CFlashMap& flashMap)
: m_IOCP(conpletitionPort)
, m_eventBreak(eventBreak)
, m_FlashMap(flashMap)
{
	m_Overlapped.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CThreadFile::~CThreadFile()
{
	::CloseHandle(m_Overlapped.hEvent);
}

void CThreadFile::PostOwnerMessage(WORD nMsg, WORD nParam, LPARAM lParam)
{
	if (IsWindow(m_hOwner)) ::PostMessage(m_hOwner, WM_FILE_THREAD_MESSAGE, MAKEWPARAM(nMsg, nParam), lParam);
}

void CThreadFile::SendOwnerMessage(WORD nMsg, WORD nParam, LPARAM lParam)
{
	if (IsWindow(m_hOwner)) ::SendMessage(m_hOwner, WM_FILE_THREAD_MESSAGE, MAKEWPARAM(nMsg, nParam), lParam);
}

void CThreadFile::InitThread(HWND hOwner, ULONGLONG nPagePerCS, WORD nCountPage, WORD nPageSize, BOOL bSeparateCS)
{
	m_hOwner = hOwner;
	m_nPagePerCS = nPagePerCS;
	m_nCountPage = nCountPage;
	m_nPageSize = nPageSize;
	m_bSeparateCS = bSeparateCS;
}

void CThreadFile::Start(BYTE nReadMode, BYTE nCS, CString strDataPath)
{
	m_nReadMode = nReadMode;
	m_nCS = nCS;
	m_strPath = strDataPath;
	StartThread();
}

int CThreadFile::Run()
{
	OVERLAPPED*	pDummy;
	LONGLONG	nOffsetCS = m_nCS * m_nPagePerCS;
	LONGLONG	nOffset;
	CBuffer*	pBuf;
	CString		strFileName;
	BYTE*		pBufRead;
	BYTE*		pData;
	BYTE*		pDataVerify;
	DWORD		nPageStart;
	DWORD		nPageEnd;
	DWORD		nMsg;
	DWORD		nBytesToTrancfer;
	DWORD		nBytesTrancfered;
	WORD		nMsgType;
	WORD		nMsgParam;
	WORD		nCountPage;
	BYTE		nState;

	//strFileName = m_strPath;
	//if (m_bSeparateCS)
	//{
	//	CString	strCS;
	//	strCS.Format(_T("_ce%d"), m_nCS);
	//	strFileName.Insert(strFileName.GetLength()-4, strCS);
	//}
	//m_hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, /*OPEN_EXISTING*/OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	m_hFile = CreateFile(m_strPath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pBufRead = (m_nReadMode == ModeVerify) ? new BYTE[m_nPageSize*m_nCountPage] : NULL;
	while (TRUE)
	{
		m_IOCP.GetStatus((ULONG_PTR*)&pBuf, &nMsg, &pDummy);
		nMsgType = LOWORD(nMsg);
		nMsgParam = HIWORD(nMsg);
		if (nMsgType == MessageThreadStep)
		{
			if (!m_eventBreak.WaitEvent(0))
			{
				pData = (BYTE*)pBuf->GetBuffer();
				nPageStart = pBuf->GetOperation();
				nCountPage = pBuf->GetUsed();
				nPageEnd = nPageStart + nCountPage;
				nOffset = (LONGLONG)nPageStart;
				if (m_bSeparateCS) nOffset -= nOffsetCS;
				nOffset *= m_nPageSize;
				nBytesToTrancfer = m_nPageSize * nCountPage;
				if (m_nReadMode == ModeFirstRead)
				{
					if (Write(pData, nOffset, nBytesToTrancfer, &nBytesTrancfered))
					{
						for (DWORD nPage = nPageStart; nPage < nPageEnd; nPage++)
						{
							m_FlashMap.SetPageState(nPage, PageStateOnce);
						}
					}
					else
					{
						nMsgType = MessageThreadError;
						nMsgParam = ErrorFileWrite;
					}
				}
				else if (m_nReadMode == ModeVerify)
				{
					if (Read(pBufRead, nOffset, nBytesToTrancfer, &nBytesTrancfered))
					{
						pDataVerify = pBufRead;
						for (DWORD nPage = nPageStart; nPage < nPageEnd; nPage++)
						{
							if (PageStateOnce == m_FlashMap.GetPageState(nPage))
							{
								nState = PageStateVerOk;
								for (DWORD nByte = 0; nByte < m_nPageSize; nByte++)
								{
									if (pDataVerify[nByte] != pData[nByte])
									{
										nState = PageStateVerErr;
										break; // for (nByte
									}
								}
								m_FlashMap.SetPageState(nPage, nState);
							}
							pDataVerify += m_nPageSize;
							pData += m_nPageSize;
						}
					}
					else
					{
						nMsgType = MessageThreadError;
						nMsgParam = ErrorFileRead;
					}
				}
				else if (m_nReadMode == ModeCorrect)
				{
					if (Write(pData, nOffset, nBytesToTrancfer, &nBytesTrancfered))
					{
						m_FlashMap.SetPageState(nPageStart, LOBYTE(nMsgParam));
					}
					else
					{
						nMsgType = MessageThreadError;
						nMsgParam = ErrorFileWrite;
					}
				}
				if (nMsgType == MessageThreadStep)
				{
					PostOwnerMessage(nMsgType, nCountPage, nPageStart);
				}
				else
				{
					m_eventBreak.SetEvent();
					PostOwnerMessage(nMsgType, nMsgParam);
				}
			}
			pBuf->Release();
		}
		else if (nMsgType == MessageThreadStart)
		{
			PostOwnerMessage(nMsgType, nMsgParam);
		}
		else if (nMsgType == MessageThreadError)
		{
			PostOwnerMessage(nMsgType, nMsgParam);
		}
		else if (nMsgType == MessageThreadStop)
		{
			if (m_eventBreak.WaitEvent(0)) nMsgParam = StopOnBreak;
			break;	// while
		}
	}
	if (pBufRead) delete pBufRead;
	::CloseHandle(m_hFile);
	PostOwnerMessage(nMsgType, nMsgParam);
	return 0;
}

BOOL CThreadFile::Read(LPVOID pBuf, LONGLONG nOffset, DWORD nBytesToRead, LPDWORD pnReadedBytes)
{
	LARGE_INTEGER	nDest;
	BOOL			bRet;
	nDest.QuadPart = nOffset;
	m_Overlapped.Offset = nDest.LowPart;
	m_Overlapped.OffsetHigh = nDest.HighPart;
	*pnReadedBytes = 0;
	bRet = ::ReadFile(m_hFile, pBuf, nBytesToRead, pnReadedBytes, &m_Overlapped);
	if (!bRet)
	{
		if (::GetLastError() == ERROR_IO_PENDING)
		{
			if (::WaitForSingleObject(m_Overlapped.hEvent, 5000) == WAIT_OBJECT_0)
			{
				bRet = ::GetOverlappedResult(m_hFile, &m_Overlapped, pnReadedBytes, FALSE);
				if (!bRet) *pnReadedBytes = 0;
			}
			else
			{
				::CancelIo(m_hFile);
			}
		}
	}
	return bRet;
}

BOOL CThreadFile::Write(LPVOID pBuf, LONGLONG nOffset, DWORD nBytesToWrite, LPDWORD pnWrittenBytes)
{
	LARGE_INTEGER	nDest;
	BOOL			bRet;
	nDest.QuadPart = nOffset;
	m_Overlapped.Offset = nDest.LowPart;
	m_Overlapped.OffsetHigh = nDest.HighPart;
	*pnWrittenBytes = 0;
	bRet = ::WriteFile(m_hFile, pBuf, nBytesToWrite, pnWrittenBytes, &m_Overlapped);
	if (!bRet)
	{
		if (::GetLastError() == ERROR_IO_PENDING)
		{
			if (::WaitForSingleObject(m_Overlapped.hEvent, 5000) == WAIT_OBJECT_0)
			{
				bRet = ::GetOverlappedResult(m_hFile, &m_Overlapped, pnWrittenBytes, FALSE);
				if (!bRet) *pnWrittenBytes = 0;
			}
			else
			{
				::CancelIo(m_hFile);
			}
		}
	}
	return bRet;
}
