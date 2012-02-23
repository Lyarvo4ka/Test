#include "StdAfx.h"
#include "FlashReader.h"
#include "FlashDatabase.h"
#include "ReplaceRecordDlg.h"
#include "ClassesExt\\FileVersion.h"
#include "ClassesExt\\IniFile.h"

#define	SIGNATURE_DATABASE	0x626C6603			// 0x03'f''l''b'

CFlashDatabase::CFlashDatabase()
{
}

CFlashDatabase::~CFlashDatabase()
{
	Clear();
}

void CFlashDatabase::Clear()
{
	for (POSITION nPosVendor = m_listVendors.GetHeadPosition(); nPosVendor != NULL; )
	{
		delete m_listVendors.GetNext(nPosVendor);
	}
	m_listVendors.RemoveAll();
	for (POSITION nPosRecord = m_listRecords.GetHeadPosition(); nPosRecord != NULL; )
	{
		delete m_listRecords.GetNext(nPosRecord);
	}
	m_listRecords.RemoveAll();
}

void CFlashDatabase::Load(CString strDatabaseName)
{
	CFileVersion	fileVer;
	CVendorRecord*	pVendor;
	CFlashRecord*	pRecord;
	CFile			fileDB;
	ULONG			nTemp;
	ULONG			nVer;
	ULONG			nVendorCount;
	ULONG			nRecordCount;
	ULONG			nFileVer = fileVer.GetFileVersionMS();

	m_strDatabaseName = strDatabaseName;
	Clear();
	if (fileDB.Open(m_strDatabaseName, CFile::modeRead))
	{
		CArchive	ar(&fileDB, CArchive::load);
		if (ar.GetFile()->GetLength() >= (4 * sizeof(ULONG)))
		{
			ar >> nTemp;
			ar >> nVer;
			ar >> nVendorCount;
			ar >> nRecordCount;
			if (nTemp == SIGNATURE_DATABASE)
			{
				if (nVer == nFileVer)
				{
					for (nTemp = 0; nTemp < nVendorCount; nTemp++)
					{
						pVendor = new CVendorRecord;
						pVendor->Serialize(ar);
						m_listVendors.AddTail(pVendor);
					}
					for (nTemp = 0; nTemp < nRecordCount; nTemp++)
					{
						pRecord = new CFlashRecord(*this);
						pRecord->Serialize(ar);
						if (!pRecord->m_nCountLU) pRecord->m_nCountLU = 1;
						m_listRecords.AddTail(pRecord);
					}
				}
			}
		}
		ar.Close();
		fileDB.Close();
	}
}

void CFlashDatabase::Import(CString strFileName)
{
	CIniFile		fileImport(strFileName);
	UINT			nVendorCount = fileImport.ReadKey(_T("Vendor"), _T("Count"), 0);
	UINT			nRecordCount = fileImport.ReadKey(_T("Flash"), _T("Count"), 0);
	CVendorRecord*	pVendor;
	CFlashRecord*	pRecord;
	CString			strKey;
	CString			strTemp;

	for (UINT nVendor = 0; nVendor < nVendorCount; nVendor++)
	{
		pVendor = new CVendorRecord;
		strKey.Format(_T("Vendor%d"), nVendor);
		fileImport.ReadKey(strKey, _T("Name"), pVendor->m_strVendor, _T(""));
		pVendor->m_nID = (BYTE)fileImport.ReadKey(strKey, _T("ID"), 0);
		AddVendor(pVendor);
	}
	for (UINT nRecord = 0; nRecord < nRecordCount; nRecord++)
	{
		pRecord = new CFlashRecord(*this);
		strKey.Format(_T("Flash%d"), nRecord);
		fileImport.ReadKey(strKey, _T("Name"), pRecord->m_strPartNumber, _T(""));
		pRecord->m_nVendorID = (BYTE)fileImport.ReadKey(strKey, _T("VendorID"), 0);
		fileImport.ReadKey(strKey, _T("ID"), strTemp, _T("0000000000000000"));
		swscanf_s(strTemp, _T("%I64X"), &pRecord->m_nID);
		fileImport.ReadKey(strKey, _T("IDMask"), strTemp, _T("0000000000000000"));
		swscanf_s(strTemp, _T("%I64X"), &pRecord->m_nMaskID);
		pRecord->m_nPageSize = (WORD)fileImport.ReadKey(strKey, _T("PageSize"), 0);
		pRecord->m_nPagePerBlock = (WORD)fileImport.ReadKey(strKey, _T("PagesPerBlock"), 0);
		pRecord->m_nBlockPerLU = (WORD)fileImport.ReadKey(strKey, _T("BlocksPerLU"), 0);
		pRecord->m_nCountLU = (BYTE)fileImport.ReadKey(strKey, _T("LUPerCS"), 0);
		pRecord->m_nCountCS = (BYTE)fileImport.ReadKey(strKey, _T("CS"), 0);
		pRecord->m_nBusWidth = (BYTE)fileImport.ReadKey(strKey, _T("Bus"), 0);
		pRecord->m_nCmdType = (BYTE)fileImport.ReadKey(strKey, _T("CmdType"), 0);
		pRecord->m_nNumCmd = (BYTE)fileImport.ReadKey(strKey, _T("NumCmd"), 0);
		pRecord->m_nNumPageAdr = (BYTE)fileImport.ReadKey(strKey, _T("PageAddr"), 0);
		pRecord->m_nNumFullAdr = (BYTE)fileImport.ReadKey(strKey, _T("FullAddr"), 0);
		pRecord->m_nSpeedAsyn = (BYTE)fileImport.ReadKey(strKey, _T("SpeedAsyn"), 0);
		pRecord->m_nPowerIO = (BYTE)fileImport.ReadKey(strKey, _T("VccIO"), 0);
		pRecord->m_nONFI = (BYTE)fileImport.ReadKey(strKey, _T("ONFI"), 0);
		if (pRecord->IsEnable()) AddRecord(pRecord);
		else delete pRecord;
	}
}

void CFlashDatabase::Update(CFlashDatabase& dbSrc)
{
	CReplaceRecordDlg	dlg;
	CVendorRecord*		pVendorSrc;
	CVendorRecord*		pVendorDst;
	CFlashRecord*		pRecordSrc;
	CFlashRecord*		pRecordDst;
	CString				strMsg;
	BOOL				bSame;
	BOOL				bAdd;

	for (POSITION nPosVendor = dbSrc.GetVendorHeadPosition(); nPosVendor != NULL; )
	{
		pVendorSrc = dbSrc.GetVendorNext(nPosVendor);
		bAdd = TRUE;
		for (POSITION nPos = m_listVendors.GetHeadPosition(); nPos != NULL; )
		{
			pVendorDst =  m_listVendors.GetNext(nPos);
			if (pVendorDst->m_strVendor == pVendorSrc->m_strVendor)
			{
				bAdd = FALSE;
				bSame = (pVendorDst->m_nID == pVendorSrc->m_nID);
				break;
			}
			if (pVendorDst->m_nID == pVendorSrc->m_nID)
			{
				bAdd = FALSE;
				bSame = FALSE;
				break;
			}
		}
		if (bAdd)
		{
			AddVendor(pVendorSrc, TRUE);
		}
		else
		{
			if (!bSame)
			{
				strMsg.Format(_T("Existing vendor:\n%s, ID %02X\nNew vendor:\n%s, ID %02X\nSKIPPED!"), pVendorDst->m_strVendor, pVendorDst->m_nID, pVendorSrc->m_strVendor, pVendorSrc->m_nID);
				AfxMessageBox(strMsg, MB_ICONEXCLAMATION|MB_OK);
			}
		}
	}
	for (POSITION nPosRecord = dbSrc.GetRecordHeadPosition(); nPosRecord != NULL; )
	{
		pRecordSrc = dbSrc.GetRecordNext(nPosRecord);
		pRecordDst = GetRecord(pRecordSrc->m_strPartNumber);
		if (pRecordDst)
		{	// Record present in database
			if (memcmp((PFlashParameters)pRecordSrc, (PFlashParameters)pRecordDst, sizeof(FlashParameters)))
			{	// Parameters block is different, Ask
				dlg.m_pRecordDst = pRecordDst;
				dlg.m_pRecordSrc = pRecordSrc;
				if (IDOK == dlg.DoModal())
				{	// Replace
					RemoveRecord(pRecordDst);
					AddRecord(pRecordSrc, TRUE);
				}
			}
		}
		else
		{	// Record not present in database, Add
			AddRecord(pRecordSrc, TRUE);
		}
	}
	Save();
}

void CFlashDatabase::Save()
{
	CFileVersion	fileVer;
	CFile			fileDB(m_strDatabaseName, CFile::modeCreate | CFile::modeWrite);
	CArchive		ar(&fileDB, CArchive::store);

	ar << (ULONG)SIGNATURE_DATABASE;
	ar << fileVer.GetFileVersionMS();
	ar << (ULONG)m_listVendors.GetCount();
	ar << (ULONG)m_listRecords.GetCount();;
	for (POSITION nPosVendor = m_listVendors.GetHeadPosition(); nPosVendor != NULL; )
	{
		m_listVendors.GetNext(nPosVendor)->Serialize(ar);
	}
	for (POSITION nPosRecord = m_listRecords.GetHeadPosition(); nPosRecord != NULL; )
	{
		m_listRecords.GetNext(nPosRecord)->Serialize(ar);
	}
	ar.Close();
	fileDB.Close();
}

UINT CFlashDatabase::GetVendorCount()
{
	return (UINT)m_listVendors.GetCount();
}

POSITION CFlashDatabase::GetVendorHeadPosition()
{
	return m_listVendors.GetHeadPosition();
}

CVendorRecord* CFlashDatabase::GetVendorNext(POSITION& nPos)
{
	return m_listVendors.GetNext(nPos);
}

void CFlashDatabase::AddVendor(CVendorRecord* pVendor, BOOL bCreateNew)
{
	CVendorRecord* pVendorNew = bCreateNew ? new CVendorRecord(pVendor) : pVendor;
	m_listVendors.AddTail(pVendorNew);
}

void CFlashDatabase::RemoveVendor(CVendorRecord* pVendor)
{
	m_listVendors.RemoveAt(m_listVendors.Find(pVendor));
}

UINT CFlashDatabase::GetRecordCount()
{
	return (UINT)m_listRecords.GetCount();
}

CFlashRecord* CFlashDatabase::GetRecord(CString strPartnumber)
{
	CFlashRecord*	pRecord;
	for (POSITION nPosRecord = m_listRecords.GetHeadPosition(); nPosRecord != NULL; )
	{
		pRecord = m_listRecords.GetNext(nPosRecord);
		if (pRecord->m_strPartNumber == strPartnumber) return pRecord;
	}
	return NULL;
}

POSITION CFlashDatabase::GetRecordHeadPosition()
{
	return m_listRecords.GetHeadPosition();
}

CFlashRecord* CFlashDatabase::GetRecordNext(POSITION& nPos)
{
	return m_listRecords.GetNext(nPos);
}

void CFlashDatabase::AddRecord(CFlashRecord* pRecord, BOOL bCreateNew)
{
	CFlashRecord*	pRecordNew = bCreateNew ? new CFlashRecord(*this, pRecord) : pRecord;
	m_listRecords.AddTail(pRecordNew);
}

void CFlashDatabase::RemoveRecord(CFlashRecord* pRecord)
{
	m_listRecords.RemoveAt(m_listRecords.Find(pRecord));
}
