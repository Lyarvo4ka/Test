#pragma once

#include "MultiThread\\MultiThread.h"
#include "FlashMap.h"
#include "Reader.h"

static const UINT WM_FILE_THREAD_MESSAGE	= RegisterWindowMessage(_T("WM_FILE_THREAD_MESSAGE"));

#define ON_FILE_THREAD_MESSAGE(memberFxn)	ON_REGISTERED_MESSAGE(WM_FILE_THREAD_MESSAGE, memberFxn)

enum MessageType
{
	MessageThreadStep,
	MessageThreadStart,
	MessageThreadError,
	MessageThreadStop
};

enum StopType
{
	StopOnComplete,
	StopOnBreak,
	StopOnError
};

enum ErrorType
{
	ErrorDeviceRead,
	ErrorFileRead,
	ErrorFileWrite,
	ErrorPower
};

class CThreadFile : public CThreadExt
{
public:

private:
	CIOCompletionPort&	m_IOCP;
	CFlashMap&			m_FlashMap;
	CManualResetEvent&	m_eventBreak;
	CString				m_strPath;
	HANDLE				m_hFile;
	OVERLAPPED			m_Overlapped;
	HWND				m_hOwner;
	ULONGLONG			m_nPagePerCS;
	WORD				m_nCountPage;
	WORD				m_nPageSize;
	BYTE				m_nReadMode;
	BYTE				m_nCS;
	BOOL				m_bSeparateCS;

public:
	CThreadFile(CIOCompletionPort& conpletitionPort, CManualResetEvent& eventBreak, CFlashMap& flashMap);
	~CThreadFile();
	void InitThread(HWND hOwner, ULONGLONG nPagePerCS, WORD nCountPage, WORD nPageSize, BOOL bSeparateCS);
	void Start(BYTE nReadMode, BYTE nCS, CString strDataPath);

protected:

private:
	virtual int Run();
	BOOL Read(LPVOID pBuf, LONGLONG nOffset, DWORD nBytesToRead, LPDWORD pnReadedBytes);
	BOOL Write(LPVOID pBuf, LONGLONG nOffset, DWORD nBytesToWrite, LPDWORD pnWrittenBytes);
	void PostOwnerMessage(WORD nMsg, WORD nParam = 0, LPARAM lParam = 0);
	void SendOwnerMessage(WORD nMsg, WORD nParam = 0, LPARAM lParam = 0);
};
