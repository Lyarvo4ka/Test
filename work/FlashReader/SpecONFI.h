#pragma once

#define	SIGNATURE_ONFI				0x49464E4F		// 'O''N''F''I'
#define	FEATURE_BUS_WIDTH_16		0x01
#define	SUPPORT_READ_CACHE			0x02
#define	VENDOR_SUPPORT_READ_CACHE	0x01

#pragma pack (1)

typedef struct _PARAMETERS_PAGE_ONFI				// Byte
{
	DWORD		Signature;							// 000-003
	WORD		RevisionNumber;						// 004-005
	WORD		FeaturesSupported;					// 006-007
	WORD		CommandsSupported;					// 008-009
	BYTE		Reserved0[2];						// 010-011
	WORD		ExtParameterPageLength;				// 012-013
	BYTE		NumberParameterPages;				// 014
	BYTE		Reserved1[17];						// 015-031
	BYTE		DeviceManufacturer[12];				// 032-043
	BYTE		DeviceModel[20];					// 044-063
	BYTE		ManufacturerID;						// 064
	WORD		DateCode;							// 065-066
	BYTE		Reserved2[13];						// 067-079
	DWORD		DataBytesPerPage;					// 080-083
	WORD		SpareBytesPerPage;					// 084-085
	DWORD		DataBytesPerPartialPage;			// 086-089
	WORD		SpareBytesPerPartialPage;			// 090-091
	DWORD		PagesPerBlock;						// 092-095
	DWORD		BlockPerLU;							// 096-099
	BYTE		NumberLU;							// 100
	BYTE		NumberAddressCycles;				// 101
	BYTE		NumberBitsPerCell;					// 102
	WORD		MaxBadBlocksPerLU;					// 103-104
	WORD		BlockEndurance;						// 105-106
	BYTE		ValidBlocksAtBeginn;				// 107
	WORD		BlockEnduranceForValidBlocks;		// 108-109
	BYTE		NumberOfProgramsPerPage;			// 110
	BYTE		PartialProgrammingAttributes;		// 111
	BYTE		NumberOfBitsECC;					// 112
	BYTE		NumberOfInterleavedAddressBits;		// 113
	BYTE		InterleavedOperationAttributes;		// 114
	BYTE		Reserved3[13];						// 115-127
	BYTE		PinCapacitance;						// 128
	WORD		AsynTimingMode;						// 129-130
	WORD		AsynProgramCacheTimingMode;			// 131-132
	WORD		MaxPageProgramTime;					// 133-134
	WORD		MaxBlockEraseTime;					// 135-136
	WORD		MaxPageReadTime;					// 137-138
	WORD		MinChangeColumnSetupTime;			// 139-140
	WORD		SynhTimingMode;						// 141-142
	BYTE		SynhFeatures;						// 143
	WORD		TypicalCapacitanceClk;				// 144-145
	WORD		TypicalCapacitancePinIO;			// 146-147
	WORD		TypicalCapacitancePinInput;			// 148-149
	BYTE		MaxCapacitancePinInput;				// 150
	BYTE		DriverStrengthSupport;				// 151
	WORD		MaxInterLeavedPageReadTime;			// 152-153
	BYTE		Reserved4[10];						// 154-163
	WORD		VendorSpecificRevisionNumber;		// 164-165
	BYTE		VendorSupportReadTwoPlane;			// 166
	BYTE		VendorSupportReadCache;				// 167
	BYTE		VendorSupportReadUniqueID;			// 168
	BYTE		VendorSupportProgrammableIO;		// 169
	BYTE		VendorNumberProgrammableIO;			// 170
	BYTE		VendorFeatureProgrammableIO;		// 171
	BYTE		VendorSupportProgrammableRB;		// 172
	BYTE		VendorFeatureProgrammableRB;		// 173
	BYTE		VendorNumberProgrammableRB;			// 174
	BYTE		VendorSupportModeOTP;				// 175
	BYTE		VendorPageStartOTP;					// 176
	BYTE		VendorAddressDataProtectOTP;		// 177
	BYTE		VendorNumberPagesOTP;				// 178
	BYTE		VendorSpecific[75];					// 179-253
	WORD		IntegrityCRC;						// 254-255
} PARAMETERS_PAGE_ONFI, *PPARAMETERS_PAGE_ONFI;

#pragma pack ()
