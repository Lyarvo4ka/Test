#pragma once

#include "SpecONFI.h"

#define BIT(x)			(1 << x)
#define	MAX_CS			4
#define	COUNT_ID_MASK	8
#define	FULL_ID_MASK	0xFFFFFFFFFFFFFFFF

#pragma pack (1)

typedef struct _FlashParameters
{
	ULONGLONG	m_nID;
	ULONGLONG	m_nMaskID;
	BYTE		m_nVendorID;
	BYTE		m_nBusWidth;
	BYTE		m_nONFI;
	BYTE		m_nReadType;		// синхронный / ассинхронный
	BYTE		m_nCmdType;			// тип команды чтения (Page/Cache)
	BYTE		m_nNumCmd;
	BYTE		m_nNumPageAdr;
	BYTE		m_nNumFullAdr;
	BYTE		m_nCountCS;
	BYTE		m_nCountLU;
	WORD		m_nBlockPerLU;
	WORD		m_nPagePerBlock;
	WORD		m_nPageSize;
	BYTE		m_nSpeedAsyn;
	BYTE		m_nSpeedSyn;
	BYTE		m_nPowerIO;
	BYTE		m_nPowerCore;
	BYTE		m_nReserved[12];
} FlashParameters, *PFlashParameters;

#pragma pack ()

enum SupportONFI
{
	Support_No,
	Support_1_0,
	Support_2_0,
	Support_2_1,
	CountSupportONFI
};

enum BusWidth
{
	Bus_16,
	Bus_08,
	CountBusWidth
};

enum PowerIO
{
	PowerIO_1_8,
	PowerIO_2_7,
	PowerIO_3_3,
	CountPowerIO
};

enum SpeedAsyn
{
	SpeedAsyn_100,
	SpeedAsyn_50,
	SpeedAsyn_35,
	SpeedAsyn_30,
	SpeedAsyn_25,
	SpeedAsyn_20,
	CountSpeedAsyn
};

enum CommandType
{
	ReadPage,
	ReadCacheNAND,
	ReadCacheONFI,
	CountCommandType
};

enum ReadType
{
	ReadOnce,
	ReadVerify,
	ReadCorrection
};

class CFlashDatabase;

class CFlashRecord : public FlashParameters
{
public:
	CFlashDatabase&		m_OwnerBase;
	CString				m_strPartNumber;

private:
	static const TCHAR*	m_strBusWidth[CountBusWidth];
	static const TCHAR*	m_strPowerIO[CountPowerIO];
	static const TCHAR*	m_strSpeed[CountSpeedAsyn];
	static const TCHAR*	m_strSupportONFI[CountSupportONFI];
	static const TCHAR*	m_strCommandType[CountCommandType];

public:
	CFlashRecord(CFlashDatabase& ownerBase, CFlashRecord* pRecord = NULL);
	~CFlashRecord();
	void Clear();
	void Copy(CFlashRecord* pRecord);
	BOOL IsParametersPageValid(PPARAMETERS_PAGE_ONFI pParametersPage);
	void SetONFI(PPARAMETERS_PAGE_ONFI pParametersPage);
	void Serialize(CArchive& ar);
	BOOL IsEnable();
	ULONG GetFirstBitLU();
	ULONGLONG GetPagePerCS();
	ULONGLONG GetPagePerLU();
	ULONGLONG GetPageCount();
	ULONGLONG GetFullSize();
	CString GetStringSize();
	CString GetStringPageSize();
	CString GetStringCommand();
	CString GetStringGeometry();
	CString GetStringVendor();
	CString GetStringVendor(ULONGLONG nSignature);
	static const CString GetStringBusWidth(BYTE nBusWidth);
	static const CString GetStringPowerIO(BYTE nPower);
	static const CString GetStringSpeed(BYTE nSpeed);
	static const CString GetStringSupportONFI(BYTE nONFI);
	static const CString GetStringCommandType(BYTE nType);
	static const CString GetStringSignature(ULONGLONG nSignature, ULONGLONG nMask = FULL_ID_MASK);

private:
	CString GetStringVendor(BYTE nID);

};


