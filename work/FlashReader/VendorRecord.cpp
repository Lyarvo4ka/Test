#include "stdafx.h"
#include "FlashReader.h"
#include "VendorRecord.h"

CVendorRecord::CVendorRecord(CVendorRecord* pVendor)
{
	pVendor ? Copy(pVendor) : Clear();
}

CVendorRecord::~CVendorRecord()
{
}

void CVendorRecord::Clear()
{
	m_strVendor.Empty();
	m_nID = 0;
}

void CVendorRecord::Copy(CVendorRecord* pVendor)
{
	m_strVendor = pVendor->m_strVendor;
	m_nID = pVendor->m_nID;
}

void CVendorRecord::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nID;
		ar << m_strVendor;
	}
	else
	{
		ar >> m_nID;
		ar >> m_strVendor;
	}
}
