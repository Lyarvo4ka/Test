#pragma once

#define MAX_PHYSICAL_DRIVE				16
#define MAX_READER_COUNT				4
#define SERIAL_NUMBER_SIZE				20
#define SHARED_DATA_SIZE				(MAX_READER_COUNT * (SERIAL_NUMBER_SIZE + 1) + 1) * sizeof(TCHAR)
#define SECTOR_SIZE						512
#define PARAMETERS_PAGE_OFFSET			0x0100
#define PARAMETERS_PAGE_COUNT			16
#define MAKE_COMMAND_PARAM(f, r, c)		(((f) & 0x0F) | (((r) & 0x0F) << 4) | (((c) & 0x0F) << 8))
#define MAKE_SPEED_PARAM(sa, ss, t, s)	(((sa) & 0x0F) | (((ss) & 0x0F) << 4) | (((t) & 0x0F) << 8) | (((s) ? 1 : 0) << 12))
#define MAKE_POWER_PARAM(i, c, o, w)	(((i) & 0x0F) | (((c) & 0x0F) << 4) | (((o) ? 1 : 0) << 8) | (((w) ? 1 : 0) << 9))
#define MAKE_READ_PARAM(c, m)			(((c) & 0x0F) | (((m == ModeCorrect) ? 0 : 1) << 4))

#pragma pack (1)
typedef struct _IDENTIFY_DISK_ATA					// ATA8 specification
{													// Word(dec)
	USHORT		GeneralConfiguration;				// 000
    USHORT		CylindersCHS;// Obsolete			// 001
    USHORT		SpecificConfiguration;				// 002
    USHORT		HeadsCHS;// Obsolete				// 003
    USHORT		Retired1[2];						// 004-005
    USHORT		SectorsCHS;// Obsolete				// 006
    USHORT		ReservedCFA1[2];					// 007-008
    USHORT		Retired2;							// 009
    UCHAR		SerialNumber[20];					// 010-019
    USHORT		Retired3[2];						// 020-021
    USHORT		NumberBytesLong;// Obsolete			// 022
    UCHAR		FirmwareRevision[8];				// 023-026
    UCHAR		ModelNumber[40];					// 027-046
    USHORT		MaxNumSectorsForMultiple;			// 047
    USHORT		TrustedComputingSupported;			// 048
    ULONG		Capabilities;						// 049-050
    USHORT		SettingsTimePIO;// Obsolete			// 051
    USHORT		SettingsTimeDMA;// Obsolete			// 052
    USHORT		SupportedModes;						// 053
    USHORT		CylindersCurrent;// Obsolete		// 054
    USHORT		HeadsCurrent;// Obsolete			// 055
    USHORT		SectorsCurrent;// Obsolete			// 056
    ULONG		CapacityCHS;// Obsolete				// 057-058
    USHORT		NumSectorsForMultiple;				// 059
    ULONG		TotalNumberLBA28;					// 060-061
    USHORT		ModeDMA;// Obsolete					// 062
    USHORT		ModeMultiwordDMA;					// 063
    USHORT		ModePIO;							// 064
    USHORT		MinCycleTimeMultiwordDMA;			// 065
    USHORT		VendorCycleTimeMultiwordDMA;		// 066
    USHORT		MinCycleTimePIOwithoutIORDY;		// 067
    USHORT		MinCycleTimePIOwithIORDY;			// 068
    USHORT		Reserved1[2];						// 069-070
    USHORT		ReservedIdentifyPacketDevice[4];	// 071-074
    USHORT		QueueDepth;							// 075
    USHORT		CapabilitiesSATA;					// 076
    USHORT		ReservedSATA;						// 077
    USHORT		FeaturesSupportedSATA;				// 078
    USHORT		FeaturesEnabledSATA;				// 079
    USHORT		MajorVersionNumber;					// 080
    USHORT		MinorVersionNumber;					// 081
    USHORT		FeatureSetSupported1;				// 082
	USHORT		FeatureSetSupported2;				// 083
    USHORT		FeatureSetSupported3;				// 084
    USHORT		FeatureSetSupportedOrEnabled1;		// 085
    USHORT		FeatureSetSupportedOrEnabled2;		// 086
    USHORT		FeatureSetSupportedOrEnabled3;		// 087
    USHORT		ModeUltraDMA;						// 088
    USHORT		TimeSecuriteEraseNormal;			// 089
    USHORT		TimeSecuriteEraseEnhanced;			// 090
    USHORT		CurrentAdvancedPowerManagementLevel;// 091
    USHORT		MasterPasswordIdentifier;			// 092
    USHORT		HardwareResetResults;				// 093
    USHORT		FeatureSetAAM;						// 094
    USHORT		StreamMinimumRequestSize;			// 095
    USHORT		StreamTransferTimeDMA;				// 096
    USHORT		StreamAccessLatency;				// 097
    ULONG		StreamingPerformanceGranularity;	// 098-099
    ULONGLONG	TotalNumberLBA48;					// 100-103
    USHORT		StreamTransferTimePIO;				// 104
	USHORT		Reserved2;							// 105
	USHORT		PhysicalAndLogicalSectorSize;		// 106
	USHORT		InterSeekDelay;						// 107
	USHORT		WorldWideName[4];					// 108-111
	USHORT		ReservedWorldWideName[4];			// 112-115
	USHORT		ReservedTLC;						// 116
	ULONG		LogicalSectorSize;					// 117-118
	USHORT		FeatureSetSupported4;				// 119
	USHORT		FeatureSetSupportedOrEnabled4;		// 120
	USHORT		ReservedSupportedAndEnabledExp[6];	// 121-126
    USHORT		RemovableMediaSupported;// Obsolete	// 127
    USHORT		SecurityStatus;						// 128
    USHORT		VendorSpecific[31];					// 129-159
    USHORT		PowerModeCFA;						// 160
    USHORT		ReservedCFA2[7];					// 161-167
    USHORT		DeviceNominalFormFactor;			// 168
    USHORT		Reserved3[7];						// 169-175
    UCHAR		MediaSerialNumber[40];				// 176-195
    UCHAR		MediaManufacturer[20];				// 196-205
    USHORT		CommandTransportSCT;				// 206
	USHORT		ReservedCE_ATA1[2];					// 207-208
    USHORT		AlignmentLogicalToPhysical;			// 209
	ULONG		WriteReadVerifySectorCountMode3;	// 210-211
	ULONG		WriteReadVerifySectorCountMode2;	// 212-213
    USHORT		NonVolatileCacheCapabilities;		// 214
	ULONG		NonVolatileCacheSize;				// 215-216
    USHORT		NominalMediaRotationRate;			// 217
    USHORT		Reserved4;							// 218
    USHORT		NonVolatileCacheOptions;			// 219
	USHORT		WriteReadVerifyCurrentMode;			// 220
    USHORT		Reserved5;							// 221
    USHORT		TransportMajorVersion;				// 222
    USHORT		TransportMinorVersion;				// 223
	USHORT		ReservedCE_ATA2[10];				// 224-233
    USHORT		MinNumberSectorsDownloadMicrocode;	// 234
    USHORT		MaxNumberSectorsDownloadMicrocode;	// 235
	USHORT		Reserved6[19];						// 236-254
    USHORT		IntegrityWord;						// 255
} IDENTIFY_DISK_ATA, *PIDENTIFY_DISK_ATA;
#pragma pack ()

enum ReaderState
{
	ReaderFree,
	ReaderBusy,
	ReaderBad
};

enum ReaderMode
{
	ModeFirstRead,
	ModeVerify,
	ModeCorrect,
	ModeCount
};

class CReader
{
public:
	int						m_nState;
	WORD					m_nMaxSectorCount;

protected:
	HANDLE					m_hDevice;
	OVERLAPPED				m_Overlapped;
	IDENTIFY_DISK_ATA		m_ReaderID;

public:
	CReader();
	~CReader();
	BOOL Open(LPCTSTR lpDeviceName);
	BOOL Close();
	CString	GetModel();
	CString	GetSerialNumber();
	CString	GetFirmware();
	CString GetCurrentMode();
	BOOL GetBoardID(LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL SetFlashParameters(WORD nBlockSize, WORD nPageSize, WORD nCountPage, WORD nCmdParam, WORD nSpeedParam, LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL SetReadParameters(WORD nReadParam, LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL PowerOn(WORD nPowerParam, LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL GetFlashID(LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL GetParametersPage(BYTE nPageNum, BYTE nCS, LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL PowerOff(LPBYTE pBuf, LPDWORD pnReadedBytes);
	BOOL ReadPage(DWORD nPage, WORD nCountLBA, LPBYTE pBuf, LPDWORD pnReadedBytes);

protected:
	void Reset();
	BOOL Read(DWORD nLBA, WORD nCountLBA, LPVOID pBuf, LPDWORD pnReadedBytes);
	CString GetStringFromID(UCHAR* pSrc, INT length);
	int Pos2Dig(BYTE nValue);
};

typedef CTypedPtrList<CPtrList, CReader*>	CReaderList;
