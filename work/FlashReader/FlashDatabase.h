#pragma once

#include "FlashRecord.h"
#include "VendorRecord.h"

typedef CTypedPtrList<CPtrList, CFlashRecord*>	CRecordList;
typedef CTypedPtrList<CPtrList, CVendorRecord*>	CVendorList;

static const UINT WM_FLASH_CHANGED	= RegisterWindowMessage(_T("WM_FLASH_CHANGED"));

#define ON_FLASH_CHANGED(memberFxn)	ON_REGISTERED_MESSAGE(WM_FLASH_CHANGED, memberFxn)
#define FLASH_RECORD_CHANGED		0
#define FLASH_PARAMETERS_CHANGED	1

class CFlashDatabase
{
public:

private:
	CRecordList		m_listRecords;
	CVendorList		m_listVendors;
	CString			m_strDatabaseName;

public:
	CFlashDatabase();
	~CFlashDatabase();
	void Load(CString strDatabaseName);
	void Import(CString strFileName);
	void Update(CFlashDatabase& dbSrc);
	void Save();
	UINT GetVendorCount();
	POSITION GetVendorHeadPosition();
	CVendorRecord* GetVendorNext(POSITION& nPos);
	void AddVendor(CVendorRecord* pVendor, BOOL bCreateNew = FALSE);
	void RemoveVendor(CVendorRecord* pVendor);
	UINT GetRecordCount();
	CFlashRecord* GetRecord(CString strPartnumber);
	POSITION GetRecordHeadPosition();
	CFlashRecord* GetRecordNext(POSITION& nPos);
	void AddRecord(CFlashRecord* pRecord, BOOL bCreateNew = FALSE);
	void RemoveRecord(CFlashRecord* pRecord);

private:
	void Clear();
};
