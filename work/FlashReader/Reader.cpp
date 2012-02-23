#include "StdAfx.h"
#include "Reader.h"
#include <WinIoCtl.h>  
#include "ntddscsi.h"

#define READER_SIGNATURE	_T("eSATA READER")
#define CMD_GET_PARAM_PAGES		0x0EF5
#define CMD_SET_READ_PARAM		0x0EF6
#define CMD_GET_BOARD_ID		0x0EF7
#define CMD_SET_BLOCK_SIZE		0x0EF8
#define CMD_SET_PAGE_SIZE		0x0EF9
#define CMD_SET_PAGE_COUNT		0x0EFA
#define CMD_SET_COMMAND_PARAM	0x0EFB
#define CMD_SET_SPEED_PARAM		0x0EFC
#define CMD_POWER_ON			0x0EFD
#define CMD_GET_FLASH_ID		0x0EFE
#define CMD_POWER_OFF			0x0EFF
#define CMD_READ_PAGE			0x0F

CReader::CReader()
{
	m_Overlapped.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	Reset();
}

CReader::~CReader()
{
	Close();
	::CloseHandle(m_Overlapped.hEvent);
}

void CReader::Reset()
{
	m_hDevice = INVALID_HANDLE_VALUE;
	m_nState = ReaderFree;
	ZeroMemory(&m_ReaderID, sizeof(m_ReaderID));
}

BOOL CReader::Open(LPCTSTR lpDeviceName)
{
	if (m_hDevice != INVALID_HANDLE_VALUE) return TRUE;
	m_hDevice = CreateFile(lpDeviceName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (m_hDevice == INVALID_HANDLE_VALUE) return FALSE;
	DWORD						nBytesReturned;
	BYTE						outBuf[SECTOR_SIZE];
	PSTORAGE_ADAPTER_DESCRIPTOR	pAdpDesc = (PSTORAGE_ADAPTER_DESCRIPTOR)outBuf;
	STORAGE_PROPERTY_QUERY		query;
	
	ZeroMemory(&m_ReaderID, sizeof(m_ReaderID));
	if (!GetBoardID((LPBYTE)&m_ReaderID, &nBytesReturned)) return Close();
	if (GetModel().Find(READER_SIGNATURE) != 0) return Close();
	m_nState = HIBYTE(m_ReaderID.ModeUltraDMA) ? ReaderFree : ReaderBad;
	query.PropertyId = StorageAdapterProperty;
	query.QueryType = PropertyStandardQuery;
	if (!DeviceIoControl(m_hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(STORAGE_PROPERTY_QUERY), &outBuf, SECTOR_SIZE, &nBytesReturned, NULL))
	{
		return Close();
	}
	m_nMaxSectorCount = (WORD)(pAdpDesc->MaximumTransferLength / SECTOR_SIZE - 1);
	return TRUE;
}

BOOL CReader::Close()
{
	if (m_hDevice != INVALID_HANDLE_VALUE) CloseHandle(m_hDevice);
	Reset();
	return FALSE;
}

CString	CReader::GetModel()
{
	return GetStringFromID(m_ReaderID.ModelNumber, sizeof(m_ReaderID.ModelNumber));
}

CString	CReader::GetSerialNumber()
{
	return GetStringFromID(m_ReaderID.SerialNumber, sizeof(m_ReaderID.SerialNumber));
}

CString	CReader::GetFirmware()
{
	return GetStringFromID(m_ReaderID.FirmwareRevision, sizeof(m_ReaderID.FirmwareRevision));
}

CString CReader::GetCurrentMode()
{
	CString	strMode;
	BOOL	bEnableUDMA = HIBYTE(m_ReaderID.ModeUltraDMA) ? TRUE : FALSE;
	BOOL	bEnableDMA = (bEnableUDMA || HIBYTE(m_ReaderID.ModeMultiwordDMA));
	if (bEnableUDMA) strMode.Format(_T("UDMA %d"), Pos2Dig(HIBYTE(m_ReaderID.ModeUltraDMA)));
	else if (bEnableDMA) strMode.Format(_T("DMA %d"), Pos2Dig(HIBYTE(m_ReaderID.ModeMultiwordDMA)));
	else strMode = _T("PIO");
	return strMode;
}

CString CReader::GetStringFromID(UCHAR* pSrc, int length)
{
	CString	str;
	CString	strTemp;
	if (pSrc[0] == 0x00) return str;
	for (int i = 0; i < length-1; i+=2)
	{
		strTemp.Format(_T("%c%c"), pSrc[i+1], pSrc[i]);
		str += strTemp;
	}
	return str.Trim();
}

int CReader::Pos2Dig(BYTE nValue)
{
	int	nDig = 0;
	BYTE	nMask = 0x01;
	while (nDig < 8)
	{
		if (nValue & nMask) return nDig;
		nDig ++;
		nMask <<= 1;
	}
	return -1;
}

BOOL CReader::Read(DWORD nLBA, WORD nCountLBA, LPVOID pBuf, LPDWORD pnReadedBytes)
{
	LARGE_INTEGER	nDest;
	BOOL			bRet;
	nDest.QuadPart = (LONGLONG)nLBA << 9;
	m_Overlapped.Offset = nDest.LowPart;
	m_Overlapped.OffsetHigh = nDest.HighPart;
	*pnReadedBytes = 0;
	bRet = ::ReadFile(m_hDevice, pBuf, (DWORD)nCountLBA << 9, pnReadedBytes, &m_Overlapped);
	if (!bRet)
	{
		if (::GetLastError() == ERROR_IO_PENDING)
		{
			if (::WaitForSingleObject(m_Overlapped.hEvent, 5000) == WAIT_OBJECT_0)
			{
				bRet = ::GetOverlappedResult(m_hDevice, &m_Overlapped, pnReadedBytes, FALSE);
				if (!bRet) *pnReadedBytes = 0;
			}
			else
			{
				::CancelIo(m_hDevice);
			}
		}
	}
	return bRet;
}

BOOL CReader::GetBoardID(LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	return Read(MAKELONG(0, CMD_GET_BOARD_ID), 1, pBuf, pnReadedBytes);
}

BOOL CReader::SetFlashParameters(WORD nBlockSize, WORD nPageSize, WORD nCountPage, WORD nCmdParam, WORD nSpeedParam, LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	if (!Read(MAKELONG(nBlockSize, CMD_SET_BLOCK_SIZE), 1, pBuf, pnReadedBytes)) return FALSE;
	if (!Read(MAKELONG(nPageSize, CMD_SET_PAGE_SIZE), 1, pBuf, pnReadedBytes)) return FALSE;
	if (!Read(MAKELONG(nCountPage, CMD_SET_PAGE_COUNT), 1, pBuf, pnReadedBytes)) return FALSE;
	if (!Read(MAKELONG(nCmdParam, CMD_SET_COMMAND_PARAM), 1, pBuf, pnReadedBytes)) return FALSE;
	return Read(MAKELONG(nSpeedParam, CMD_SET_SPEED_PARAM), 1, pBuf, pnReadedBytes);
}

BOOL CReader::SetReadParameters(WORD nReadParam, LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	return Read(MAKELONG(nReadParam, CMD_SET_READ_PARAM), 1, pBuf, pnReadedBytes);
}

BOOL CReader::PowerOn(WORD nPowerParam, LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	return Read(MAKELONG(nPowerParam, CMD_POWER_ON), 1, pBuf, pnReadedBytes);
}

BOOL CReader::GetFlashID(LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	return Read(MAKELONG(0, CMD_GET_FLASH_ID), 1, pBuf, pnReadedBytes);
}

BOOL CReader::GetParametersPage(BYTE nPageNum, BYTE nCS, LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	return Read(MAKELONG(MAKEWORD(nPageNum, nCS), CMD_GET_PARAM_PAGES), 1, pBuf, pnReadedBytes);
}

BOOL CReader::PowerOff(LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	return Read(MAKELONG(0, CMD_POWER_OFF), 1, pBuf, pnReadedBytes);
}

BOOL CReader::ReadPage(DWORD nPage, WORD nCountLBA, LPBYTE pBuf, LPDWORD pnReadedBytes)
{
	*((BYTE*)&nPage + 3) = CMD_READ_PAGE;
	return Read(nPage, nCountLBA, pBuf, pnReadedBytes);
}
