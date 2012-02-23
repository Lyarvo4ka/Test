#include "stdafx.h"
#include "FlashReader.h"
#include "FlashRecord.h"
#include "FlashDatabase.h"

const TCHAR* CFlashRecord::m_strBusWidth[CountBusWidth] =
{
	_T("16 bit"),
	_T(" 8 bit")
};

const TCHAR* CFlashRecord::m_strPowerIO[CountPowerIO] =
{
	_T("1,8 V"),
	_T("2,7 V"),
	_T("3,3 V")
};

const TCHAR* CFlashRecord::m_strSpeed[CountSpeedAsyn] =
{
	_T("100 nS"),
	_T(" 50 nS"),
	_T(" 35 nS"),
	_T(" 30 nS"),
	_T(" 25 nS"),
	_T(" 20 nS")
};

const TCHAR* CFlashRecord::m_strSupportONFI[CountSupportONFI] =
{
	_T(" No"),
	_T("1.0"),
	_T("2.0"),
	_T("2.1"),
};

const TCHAR* CFlashRecord::m_strCommandType[CountCommandType] =
{
	_T("ReadPage"),
	_T("ReadCacheNAND"),
	_T("ReadCacheONFI"),
};

CFlashRecord::CFlashRecord(CFlashDatabase& ownerBase, CFlashRecord* pRecord)
: m_OwnerBase(ownerBase)
{
	pRecord ? Copy(pRecord) : Clear();
}

CFlashRecord::~CFlashRecord()
{
}

void CFlashRecord::Clear()
{
	m_strPartNumber.Empty();
	ZeroMemory((PFlashParameters)this, sizeof(FlashParameters));
	m_nCountCS = 1;
	m_nCountLU = 1;
	m_nNumCmd = 2;
	m_nNumFullAdr = 5;
	m_nNumPageAdr = 3;
	m_nBusWidth = Bus_08;
	m_nPowerIO = PowerIO_1_8;
	m_nSpeedAsyn = SpeedAsyn_100;
	m_nMaskID = FULL_ID_MASK;
}

void CFlashRecord::Copy(CFlashRecord* pRecord)
{
	m_strPartNumber = pRecord->m_strPartNumber;
	memcpy((PFlashParameters)this, (PFlashParameters)pRecord, sizeof(FlashParameters));

}

BOOL CFlashRecord::IsParametersPageValid(PPARAMETERS_PAGE_ONFI pParametersPage)
{
	if (pParametersPage->Signature != SIGNATURE_ONFI) return FALSE;
	BYTE*	pPsp = (BYTE*)pParametersPage;
	WORD	nCRC = 0x4F4E;							// Initialize the shift register with 0x4F4E
	WORD	nBit;
	for (int i = 0; i < sizeof(PARAMETERS_PAGE_ONFI) - 2; i++)
	{	// Input byte stream, one byte at a time, bits processed from MSB to LSB
		for (WORD nBitMask = 0x80; nBitMask; nBitMask >>= 1)
		{
			nBit = nCRC & 0x8000;
			nCRC <<= 1;
			if (pPsp[i] & nBitMask) nBit ^= 0x8000;
			if (nBit) nCRC ^= 0x8005;				// Polynomial
		}
	}
	return (nCRC == pParametersPage->IntegrityCRC);
}

void CFlashRecord::SetONFI(PPARAMETERS_PAGE_ONFI pParametersPage)
{
	CString	strTemp;
	int		nInd;

	m_strPartNumber.Empty();
	for (int i = 0; i < sizeof(pParametersPage->DeviceModel); i++)
	{
		strTemp.Format(_T("%c"), pParametersPage->DeviceModel[i]);
		m_strPartNumber += strTemp;
	}
	nInd = m_strPartNumber.Find(_T(" "));
	if (nInd != -1) m_strPartNumber = m_strPartNumber.Left(nInd);
	for (int i = Support_No; i < CountSupportONFI; i++)
	{
		if (pParametersPage->RevisionNumber & BIT(i)) m_nONFI = i;
	}
	m_nVendorID = pParametersPage->ManufacturerID;
	m_nCountLU = pParametersPage->NumberLU;
	m_nBlockPerLU = (WORD)pParametersPage->BlockPerLU;
	m_nPagePerBlock = (WORD)pParametersPage->PagesPerBlock;
	m_nPageSize = (WORD)(pParametersPage->DataBytesPerPage + pParametersPage->SpareBytesPerPage);
	m_nNumCmd = 2;
	m_nNumPageAdr = pParametersPage->NumberAddressCycles & 0x0F;
	m_nNumFullAdr = m_nNumPageAdr + (pParametersPage->NumberAddressCycles >> 4);
	m_nBusWidth = (pParametersPage->FeaturesSupported & FEATURE_BUS_WIDTH_16) ? Bus_16 : Bus_08;
	for (int i = SpeedAsyn_100; i < CountSpeedAsyn; i++)
	{
		if (pParametersPage->AsynTimingMode & BIT(i)) m_nSpeedAsyn = i;
	}
	if ((pParametersPage->CommandsSupported & SUPPORT_READ_CACHE) || (pParametersPage->VendorSupportReadCache & VENDOR_SUPPORT_READ_CACHE)) m_nCmdType = ReadCacheONFI;
}

void CFlashRecord::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strPartNumber;
		ar.Write((PFlashParameters)this, sizeof(FlashParameters));
	}
	else
	{
		ar >> m_strPartNumber;
		ar.Read((PFlashParameters)this, sizeof(FlashParameters));
	}
}

BOOL CFlashRecord::IsEnable()
{
	return (!m_strPartNumber.IsEmpty() && GetFullSize());
}

ULONG CFlashRecord::GetFirstBitLU()
{
	ULONG		nBitLU;
	ULONG		nBit = 0x00000001;
	ULONGLONG	nPagePerLU = GetPagePerLU();
	ULONG		nCnt = 0;
	for (UINT i = 0; i < sizeof(nBit) * 8; i++)
	{
		if (nPagePerLU & nBit)
		{
			nBitLU = nBit;
			nCnt++;
		}
		nBit <<= 1;
	}
	if (nCnt > 1) nBitLU <<= 1;
	return nBitLU;
}

ULONGLONG CFlashRecord::GetPagePerLU()
{
	return (ULONG)m_nBlockPerLU * m_nPagePerBlock;
}

ULONGLONG CFlashRecord::GetPagePerCS()
{
	return GetPagePerLU() * m_nCountLU;
}

ULONGLONG CFlashRecord::GetPageCount()
{
	return GetPagePerCS() * m_nCountCS;
}

ULONGLONG CFlashRecord::GetFullSize()
{
	return (ULONGLONG)m_nPageSize * GetPageCount();
}

CString CFlashRecord::GetStringSize()
{
	WORD		nPageSize = (m_nPageSize / 512) * 512;
	ULONGLONG	nSize = GetPageCount() * (nPageSize) / 1048576;	// size in MB
	FLOAT		nSizeF = (FLOAT)nSize / (FLOAT)1024;
	CString		strSize;
	strSize.Format(_T("%.3f"), (FLOAT)nSize/((nSize < 1024) ? 1000 : 1024));	// size in GB
	return strSize;
}

CString CFlashRecord::GetStringPageSize()
{
	CString	strPageSize;
	INT		nLen;
	strPageSize.Format(_T("%d B"), m_nPageSize);
	nLen = strPageSize.GetLength();
	for (INT i = 0; i < 6 - nLen; i++)
	{
		strPageSize.Insert(0, _T(" "));
	}
	return strPageSize;
}

CString CFlashRecord::GetStringCommand()
{
	CString	strTemp;

	strTemp = _T("<00>");
	for (int i = 0; i < (m_nNumFullAdr - m_nNumPageAdr); i++) strTemp += _T("<Ac>");
	for (int i = 0; i < m_nNumPageAdr; i++) strTemp += _T("<Ap>");
	if (m_nNumCmd == 2)
	{
		strTemp += ((m_nCmdType == ReadCacheNAND) ? _T("<31>") : _T("<30>"));
		if (m_nCmdType)
		{
			strTemp += _T("..<31>..");
			strTemp += ((m_nCmdType == ReadCacheNAND) ? _T("<34>") : _T("<3F>"));
		}
	}
	return strTemp;
}

CString CFlashRecord::GetStringGeometry()
{
	CString	strTemp;

	strTemp.Format(_T("%d CE x %d LU x %d Block x %d Page x %d Byte"), m_nCountCS, m_nCountLU, m_nBlockPerLU, m_nPagePerBlock, m_nPageSize);
	return strTemp;
}

const CString CFlashRecord::GetStringBusWidth(BYTE nBusWidth)
{
	return nBusWidth < CountBusWidth ? m_strBusWidth[nBusWidth] : _T("");
}

const CString CFlashRecord::GetStringPowerIO(BYTE nPower)
{
	return nPower < CountPowerIO ? m_strPowerIO[nPower] : _T("");
}

const CString CFlashRecord::GetStringSpeed(BYTE nSpeed)
{
	return nSpeed < CountSpeedAsyn ? m_strSpeed[nSpeed] : _T("");
}

const CString CFlashRecord::GetStringSupportONFI(BYTE nONFI)
{
	return nONFI < CountSupportONFI ? m_strSupportONFI[nONFI] : _T("");
}

const CString CFlashRecord::GetStringCommandType(BYTE nType)
{
	return nType < CountCommandType ? m_strCommandType[nType] : _T("");
}

const CString CFlashRecord::GetStringSignature(ULONGLONG nSignature, ULONGLONG nMask)
{
	CString		strSignature;
	if (nSignature)
	{
		CString	strByte;
		BYTE*	pSignature = (BYTE*)&nSignature;
		BYTE*	pMask = (BYTE*)&nMask;
		for (int i = 7; i >= 0; i--)
		{
			if (pMask[i])
			{
				strByte.Format(_T("%02X"), pSignature[i]);
			}
			else
			{
				strByte = strSignature.IsEmpty() ? _T("") : _T("xx");
			}
			if (!strSignature.IsEmpty()) strByte += _T(" ");
			strSignature = strByte + strSignature;
		}
	}
	return strSignature;
}

CString CFlashRecord::GetStringVendor(BYTE nID)
{
	if (!nID) return _T("");
	CVendorRecord*	pVendor;
	for (POSITION nPosVendor = m_OwnerBase.GetVendorHeadPosition(); nPosVendor != NULL; )
	{
		pVendor = m_OwnerBase.GetVendorNext(nPosVendor);
		if (pVendor->m_nID == nID) return pVendor->m_strVendor;
	}
	return _T("UNKNOWN");
}

CString CFlashRecord::GetStringVendor()
{
	return GetStringVendor(m_nVendorID);
}

CString	CFlashRecord::GetStringVendor(ULONGLONG nSignature)
{
	BYTE	nID = *(BYTE*)&nSignature;
	return GetStringVendor(nID);
}
