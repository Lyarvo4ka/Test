#pragma once

class CVendorRecord
{
public:
	CString	m_strVendor;
	BYTE	m_nID;

public:
	CVendorRecord(CVendorRecord* pVendor = NULL);
	~CVendorRecord();
	void Clear();
	void Copy(CVendorRecord* pVendor);
	void Serialize(CArchive& ar);
};


