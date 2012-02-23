#include "stdafx.h"
#include "FlashReader.h"
#include "FlashReaderDoc.h"
#include "MainFrm.h"
#include "SelectReaderDlg.h"
#include "BaseEditDlg.h"
#include "SettingsDlg.h"
#include "NewTaskDlg.h"
#include "ClassesExt\\FileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	SIGNATURE_FAIL				0x4C494146			// 'F''A''I''L'
#define	SIGNATURE_NO_CS				0x00000000			//
#define	SIGNATURE_TASK				0x646C6603			// 0x03'f''l''d'
#define	POWER_FAIL_DEVICE			0xAAAAAAAA			//
#define	POWER_FAIL_HIGH				0xBBBBBBBB			//
#define	POWER_FAIL_LOW				0xCCCCCCCC			//
#define	DIRECTORY_DATA				_T("\\Data")
#define	EXTANSION_TASK				_T(".fld")
#define	EXTANSION_DATABASE			_T(".flb")
#define	EXTANSION_IMPORT			_T(".fli")
#define	EXTANSION_DATA				_T(".dat")
#define	EXTANSION_LOG				_T(".log")
#define	PROFILE_SETINGS				_T("Settings")
#define	PROFILE_DATAPATH			_T("Data Path")
#define	PROFILE_CORRECTION_LIMIT	_T("Correction Limit")
#define	PROFILE_CORRECTION_DELTA	_T("Correction Delta")
#define	PROFILE_CORRECTION_STEP		_T("Correction Step Count")
#define	PROFILE_LOG_ENABLE			_T("Log file Enable")
#define	PROFILE_LOG_LENGTH			_T("Log file Max length")
#define	PROFILE_SEPARATE_CS			_T("Separate data files")

IMPLEMENT_DYNCREATE(CFlashReaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CFlashReaderDoc, CDocument)
	ON_COMMAND(ID_READER_CONNECT, &CFlashReaderDoc::OnReaderConnect)
	ON_COMMAND(ID_READER_START, &CFlashReaderDoc::OnReaderStart)
	ON_COMMAND(ID_READER_STOP, &CFlashReaderDoc::OnReaderStop)
	ON_COMMAND(ID_READER_SETTINGS, &CFlashReaderDoc::OnReaderSettings)
	ON_COMMAND(ID_DATABASE_EDIT, &CFlashReaderDoc::OnDatabaseEdit)
	ON_COMMAND(ID_DATABASE_IMPORT, &CFlashReaderDoc::OnDatabaseImport)
	ON_COMMAND(ID_DATABASE_MERGE, &CFlashReaderDoc::OnDatabaseMerge)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CFlashReaderDoc::OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CFlashReaderDoc::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_READER_CONNECT, &CFlashReaderDoc::OnUpdateReaderConnect)
	ON_UPDATE_COMMAND_UI(ID_READER_START, &CFlashReaderDoc::OnUpdateReaderStart)
	ON_UPDATE_COMMAND_UI(ID_READER_STOP, &CFlashReaderDoc::OnUpdateReaderStop)
	ON_UPDATE_COMMAND_UI(ID_READER_SETTINGS, &CFlashReaderDoc::OnUpdateReaderSettings)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_EDIT, &CFlashReaderDoc::OnUpdateDatabaseEdit)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_IMPORT, &CFlashReaderDoc::OnUpdateDatabaseImport)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_MERGE, &CFlashReaderDoc::OnUpdateDatabaseMerge)
END_MESSAGE_MAP()

CFlashReaderDoc::CFlashReaderDoc()
: m_threadRead(m_FlashMap)
, m_Flash(m_Database)
{
	m_bEnableNewTask = FALSE;
	m_bStarted = FALSE;
	m_pFrameWnd = NULL;
	m_pReader = NULL;
	m_Database.Load(GetDatabaseName());
	m_strDataDirectoty = theApp.GetProfileString(PROFILE_SETINGS, PROFILE_DATAPATH, _T(""));
	if (!PathIsDirectory(m_strDataDirectoty)) m_strDataDirectoty.Empty();
	if (m_strDataDirectoty.IsEmpty())
	{
		m_strDataDirectoty = GetAppDirectory() + DIRECTORY_DATA;
		::CreateDirectory(m_strDataDirectoty, NULL);
	}
	m_nLimit = theApp.GetProfileInt(PROFILE_SETINGS, PROFILE_CORRECTION_LIMIT, LIMIT_DEFAULT);
	m_nDelta = theApp.GetProfileInt(PROFILE_SETINGS, PROFILE_CORRECTION_DELTA, DELTA_DEFAULT);
	m_nStepCount = theApp.GetProfileInt(PROFILE_SETINGS, PROFILE_CORRECTION_STEP, STEP_COUNT_DEFAULT);
	m_bLogToFile = (BOOL)theApp.GetProfileInt(PROFILE_SETINGS, PROFILE_LOG_ENABLE, 0);
	m_bSeparateCS = (BYTE)theApp.GetProfileInt(PROFILE_SETINGS, PROFILE_SEPARATE_CS, 0);
	m_nLogFileMaxLengthInMB = theApp.GetProfileInt(PROFILE_SETINGS, PROFILE_LOG_LENGTH, 1);
	OpenLogFile();
}

CFlashReaderDoc::~CFlashReaderDoc()
{
	CloseLogFile();
}

#ifdef _DEBUG
void CFlashReaderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFlashReaderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CFlashReaderDoc::DeleteContents()
{
	if (!m_pFrameWnd)
	{
		POSITION	pos = GetFirstViewPosition();
		CView*		pView = GetNextView(pos);	
		m_pFrameWnd = (CMainFrame*)pView->GetParentFrame();
	}
	if (IsModified()) DoSave(m_strTaskName);
	m_Flash.Clear();
	m_FlashMap.Clear();
	DisableAllMode();
	m_nMaskCS = 0;
	m_nSignature = 0;
	UpdateAllViews(NULL, UpdateLogClear);
	CDocument::DeleteContents();
}

BOOL CFlashReaderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;
	if (!m_bEnableNewTask)
	{
		ConnectReader(FALSE);
		m_bEnableNewTask = TRUE;
		return TRUE;
	}
	CNewTaskDlg		dlgTask(this, m_pFrameWnd);
	dlgTask.m_strTaskName = m_strDataDirectoty + _T("\\");
	if (IDCANCEL == dlgTask.DoModal()) return TRUE;
	m_nTaskMode = ModeFirstRead;
	m_Flash.Copy(&dlgTask.m_Flash);
	m_FlashMap.Create(m_Flash.GetPageCount());
	SetMode(ModeFirstRead, m_nTaskMode, StateWait);
	m_strTaskName = dlgTask.m_strTaskName + EXTANSION_TASK;
	InitTask();
	return TRUE;
}

BOOL CFlashReaderDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName)) return FALSE;
	m_strTaskName = lpszPathName;
	if (!m_bEnableNewTask)
	{
		ConnectReader(FALSE);
		m_bEnableNewTask = TRUE;
	}
	InitTask(FALSE);
	return TRUE;
}

void CFlashReaderDoc::Serialize(CArchive& ar)
{
	CFileVersion	fileVer;
	ULONG			nFileVer = fileVer.GetFileVersionMS();
	ULONG			nTemp = 0;
	if (ar.IsStoring())
	{	// Storing code
		ar << (ULONG)SIGNATURE_TASK;
		ar << fileVer.GetFileVersionMS();
		ar << nTemp;
		ar << nTemp;
		ar << MAKELONG(MAKEWORD(m_nTaskMode, m_bSeparateCS), 0);
		ar.Write(m_nSubTaskState , sizeof(m_nSubTaskState));
		m_Flash.Serialize(ar);
		m_FlashMap.Serialize(ar);
	}
	else
	{	// Loading code	
		ar >> nTemp;
		if (nTemp == SIGNATURE_TASK)
		{
			ar >> nFileVer;
			if (nFileVer == nFileVer)
			{
				ar >> nTemp;
				ar >> nTemp;
				ar >> nTemp;
				m_nTaskMode = LOBYTE(LOWORD(nTemp));
				m_bSeparateCS = HIBYTE(LOWORD(nTemp));
				ar.Read(m_nSubTaskState , sizeof(m_nSubTaskState));
				m_Flash.Serialize(ar);
				if (!m_Flash.m_nCountLU) m_Flash.m_nCountLU = 1;
				m_FlashMap.Serialize(ar);
			}
			else
			{
				AfxMessageBox(IDS_WRONG_TASK_VERSION, MB_OK|MB_ICONEXCLAMATION);
			}
		}
		else
		{
			AfxMessageBox(IDS_WRONG_TASK_FILE, MB_OK|MB_ICONEXCLAMATION);
		}
	}
}


BOOL CFlashReaderDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	m_threadRead.Stop();
	m_threadRead.WaitStopComplete();
	if (m_pReader) delete m_pReader;
	return CDocument::CanCloseFrame(pFrame);
}

void CFlashReaderDoc::OnReadThreadMessage(DWORD nMsg, ULONG nMsgData)
{
	CString			strLog;
	WORD			nMsgType = LOWORD(nMsg);
	WORD			nMsgParam = HIWORD(nMsg);
	static ULONG	nStepCount;
	static CTime	timeStart;
	if (nMsgType == MessageThreadStep)
	{
		nStepCount++;
		UpdateAllViews(NULL, MAKELPARAM(UpdateStepProgress, nMsgParam), (CObject*)&nMsgData);
	}
	else if (nMsgType == MessageThreadStart)
	{
		CString	strMode;
		strMode.LoadString(IDS_READ_ONCE + m_nCurrentMode);
		strLog.Format(IDS_SUBTASK_START, m_nCurrentCS, strMode);
		UpdateLogString(strLog);
		UpdateAllViews(NULL, UpdateStepStart);
		timeStart = CTime::GetCurrentTime();
		nStepCount = 0;
	}
	else if (nMsgType == MessageThreadError)
	{
		UINT	nStrID;
		switch (nMsgParam)
		{
		case ErrorDeviceRead:
			nStrID = IDS_ERROR_READ_FLASH;
			break;
		case ErrorFileRead:
			nStrID = IDS_ERROR_READ_FILE;
			break;
		case ErrorFileWrite:
			nStrID = IDS_ERROR_WRITE_FILE;
			break;
		case ErrorPower:
			nStrID = IDS_ERROR_POWER;
			break;
		}
		strLog.LoadString(nStrID);
		UpdateLogString(strLog);
	}
	else if (nMsgType == MessageThreadStop)
	{
		if (nMsgParam == StopOnError)
		{
			m_bStarted = FALSE;
			strLog.LoadString(IDS_SUBTASK_ERROR);
		}
		else if (nMsgParam == StopOnBreak)
		{
			m_bStarted = FALSE;
			strLog.LoadString(IDS_SUBTASK_BREAK);
		}
		else //nMsgParam == StopOnComplete
		{
			CTime		timeEnd = CTime::GetCurrentTime();
			CTimeSpan	timeElapsed(timeEnd - timeStart);
			LONGLONG	nTotalSeconds = timeElapsed.GetTotalSeconds();
			LONGLONG	nSpeed = nTotalSeconds ? ((LONGLONG)m_Flash.m_nPageSize * ((m_nCurrentMode == ReadCorrection) ? 1 : m_threadRead.GetReadPageCount()) * nStepCount * 60 / 1048576 / nTotalSeconds) : 0;
			strLog.Format(IDS_SUBTASK_COMPLETE, (LONG)timeElapsed.GetTotalMinutes(), timeElapsed.GetSeconds(), (LONG)nSpeed);
		}
		UpdateLogString(strLog);
		m_nSubTaskState[m_nCurrentMode*MAX_CS + m_nCurrentCS] = m_bStarted ? StateComplete : StateWait;
		UpdateAllViews(NULL, UpdateStepEnd);
		DoSave(m_strTaskName);
		if (m_bStarted) StartNext();
	}
}

BOOL CFlashReaderDoc::CheckFlash(ULONGLONG& nSignature, BYTE& nMaskCS, CFlashRecord& flash, BYTE* pBuf)
{
	BYTE	bufDummy[SECTOR_SIZE];
	DWORD	nReadedBytes;
	BYTE*	pBufTemp;
	BOOL	bONFI = FALSE;

	nMaskCS = 0;
	nSignature = 0;
	m_pReader->PowerOn(MAKE_POWER_PARAM(flash.m_nPowerIO, flash.m_nPowerCore, flash.m_nONFI, flash.m_nBusWidth), bufDummy, &nReadedBytes);
	DWORD	nPowerState = *(DWORD*)bufDummy;
	if (nPowerState)
	{
		m_pReader->PowerOff(bufDummy, &nReadedBytes);
		//flash.Clear();
		CString	strLog;
		if (nPowerState == POWER_FAIL_DEVICE)
		{
			strLog.LoadString(IDS_POWER_FAIL);
		}
		else if (nPowerState == POWER_FAIL_HIGH)
		{
			strLog.LoadString(IDS_POWER_HIGH);
		}
		else if (nPowerState == POWER_FAIL_LOW)
		{
			strLog.LoadString(IDS_POWER_LOW);
		}
		else
		{
			strLog.LoadString(IDS_POWER_UNKNOWN);
		}
		AfxMessageBox(strLog, MB_ICONEXCLAMATION | MB_OK);
		UpdateLogString(strLog);
		return FALSE;
	}
	m_pReader->GetFlashID(pBuf, &nReadedBytes);
	m_pReader->PowerOff(bufDummy, &nReadedBytes);
	//
	//memcpy(bufDummy, pBuf, SECTOR_SIZE);
	//
	for (int nCS = 0; nCS < MAX_CS; nCS++)
	{
		pBufTemp = pBuf;
		if (*(DWORD*)pBufTemp == SIGNATURE_FAIL) pBufTemp += 0x40;
		for (int i = 1; i < 8; i++)
		{
			if (pBufTemp[i] != *pBufTemp)
			{
				nMaskCS |= BIT(nCS);
				if (!nSignature) nSignature = *(ULONGLONG*)pBufTemp;
				if (*((DWORD*)(pBuf + 0x80)) == SIGNATURE_ONFI) bONFI = TRUE;
				break;
			}
		}
		pBuf += 16;
	}
	return bONFI;
}

BOOL CFlashReaderDoc::ReadAdditionalParametersPages(CFlashRecord& flash, BYTE* pBuf)
{
	BYTE					bufDummy[SECTOR_SIZE];
	DWORD					nReadedBytes;
	BOOL					bSuccess = FALSE;

	m_pReader->PowerOn(MAKE_POWER_PARAM(flash.m_nPowerIO, flash.m_nPowerCore, flash.m_nONFI, flash.m_nBusWidth), bufDummy, &nReadedBytes);
	for (BYTE nCS = 0; nCS < flash.m_nCountCS; nCS++)
	{
		for (BYTE nPage = 1; nPage < PARAMETERS_PAGE_COUNT; nPage++)
		{
			m_pReader->GetParametersPage(nPage, nCS, pBuf, &nReadedBytes);
			if (flash.IsParametersPageValid((PPARAMETERS_PAGE_ONFI)pBuf))
			{
				bSuccess = TRUE;
				break;
			}
		}
		if (bSuccess) break;
	}
	m_pReader->PowerOff(bufDummy, &nReadedBytes);
	return bSuccess;
}

BOOL CFlashReaderDoc::CheckNewFlash(ULONGLONG& nSignature, BYTE& nMaskCS, CFlashRecord& flash)
{
	BYTE					bufCheck[SECTOR_SIZE];
	PPARAMETERS_PAGE_ONFI	pParametersONFI;
	BOOL					bONFI;
	BOOL					bSuccess;

	while (true)
	{
		bONFI = CheckFlash(nSignature, nMaskCS, flash, bufCheck);
		if (nMaskCS)
		{
			flash.m_nCountCS = 0;
			for (int nCS = 0; nCS < MAX_CS; nCS++)
			{
				if (nMaskCS & BIT(nCS)) flash.m_nCountCS++;
			}
			flash.m_nID = nSignature;
			flash.m_nMaskID = FULL_ID_MASK;
			flash.m_nVendorID = *(BYTE*)&nSignature;
			bSuccess = TRUE;
			if (bONFI)
			{
				pParametersONFI = (PPARAMETERS_PAGE_ONFI)(bufCheck + PARAMETERS_PAGE_OFFSET);
				if (!flash.IsParametersPageValid(pParametersONFI))
				{
					pParametersONFI = (PPARAMETERS_PAGE_ONFI)bufCheck;
					bSuccess = ReadAdditionalParametersPages(flash, bufCheck);
				}
				if (bSuccess)
				{
					flash.SetONFI(pParametersONFI);
				}
				else
				{
					bSuccess = TRUE;
					AfxMessageBox(IDS_WRONG_PARAMETERS_PAGE, MB_ICONEXCLAMATION | MB_OK);
				}
			}
			break;
		}
		else
		{
			if (flash.m_nBusWidth != Bus_16)
			{
				flash.m_nBusWidth = Bus_16;
			}
			else
			{
				bSuccess = FALSE;
				break;
			}
		}
	}
	return bSuccess;
}

BOOL CFlashReaderDoc::CheckExistedFlash(ULONGLONG& nSignature, BYTE& nMaskCS, CFlashRecord& flash)
{
	BYTE	bufCheck[SECTOR_SIZE];
	BOOL	bSuccess;
	CString	strLog;
	CString	strTemp;

	if (!IsConnected() || !m_bEnableNewTask) return FALSE;
	CheckFlash(nSignature, nMaskCS, flash, bufCheck);
	if (nMaskCS)
	{
		strTemp.Empty();
		for (int nCS = 0; nCS < flash.m_nCountCS; nCS++)
		{
			if (!(nMaskCS & BIT(nCS)))
			{
				strLog.Format(_T("CE%d, "), nCS);
				strTemp += strLog;
			}
		}
		if (strTemp.IsEmpty())
		{
			if ((nSignature & flash.m_nMaskID) == (flash.m_nID & flash.m_nMaskID))
			{
				strTemp = _T("OK");
				m_nMaskID = flash.m_nMaskID;
				bSuccess = TRUE;
			}
			else
			{
				strTemp = _T("Wrong Flash");
				m_nMaskID = FULL_ID_MASK;
				bSuccess = FALSE;
			}
		}
		else
		{
			strTemp.Truncate(strLog.GetLength()-2);
			strTemp += _T(" - FAIL");
			m_nMaskID = FULL_ID_MASK;
			bSuccess = FALSE;
		}
		strLog.Format(IDS_CHECK_FLASH, strTemp, CFlashRecord::GetStringSignature(nSignature, m_nMaskID));
	}
	else
	{
		strLog.LoadString(IDS_ERROR_NO_FLASH);
		bSuccess = FALSE;
	}
	if (!bSuccess) AfxMessageBox(strLog, MB_OK|MB_ICONSTOP);
	UpdateLogString(strLog);
	UpdateAllViews(NULL, UpdateCheck);
	return bSuccess;
}

void CFlashReaderDoc::PrepareDataFile(CString strFileName, ULONGLONG nSize, BOOL bNew)
{
	CFile		fileData;
	fileData.Open(strFileName, CFile::modeCreate|(bNew ? 0 : CFile::modeNoTruncate)|CFile::modeReadWrite);
	if (bNew)
	{
		fileData.SetLength(nSize);
	}
	else
	{
		if (nSize != fileData.GetLength())
		{
			AfxMessageBox(IDS_WRONG_FILE, MB_OK|MB_ICONEXCLAMATION);
			fileData.SetLength(0);
			fileData.SetLength(nSize);
			m_FlashMap.SetAllPages(PageStateNo);
			DisableAllMode();
			SetMode(ModeFirstRead, m_nTaskMode, StateWait);
		}
	}
	fileData.Close();
}

void CFlashReaderDoc::InitTask(BOOL bNew)
{
	CString		strLog;
	ULONGLONG	nFileSize = m_Flash.GetFullSize();

	// Prepare data file(s)
	if (m_bSeparateCS)
	{
		nFileSize /= m_Flash.m_nCountCS;
		for (BYTE nCS = 0; nCS < m_Flash.m_nCountCS; nCS++)
		{
			PrepareDataFile(GetDataFileName(nCS), nFileSize, bNew);
		}
	}
	else
	{
		PrepareDataFile(GetDataFileName(), nFileSize, bNew);
	}
	// Log task parameters
	strLog.Format(bNew ? IDS_TASK_NEW : IDS_TASK_LOAD, PathFindFileName(m_strTaskName));
	UpdateLogString(strLog);
	strLog.Format(_T("\tChip\t\t%s - %s\n"), m_Flash.m_strPartNumber, m_Flash.GetStringVendor());
	StringToLogFile(strLog);
	strLog.Format(_T("\tGeometry\t%s = %s\n"), m_Flash.GetStringGeometry(), m_Flash.GetStringSize());
	StringToLogFile(strLog);
	strLog.Format(_T("\tBus width\t%s\n"), m_Flash.GetStringBusWidth(m_Flash.m_nBusWidth));
	StringToLogFile(strLog);
	strLog.Format(_T("\tRead cycle\t%s\n"), m_Flash.GetStringSpeed(m_Flash.m_nSpeedAsyn));
	StringToLogFile(strLog);
	strLog.Format(_T("\tVCC IO\t\t%s\n"), m_Flash.GetStringPowerIO(m_Flash.m_nPowerIO));
	StringToLogFile(strLog);
	strLog.Format(_T("\tCommand format\t%s\n"), m_Flash.GetStringCommand());
	StringToLogFile(strLog);
	if (bNew) DoSave(m_strTaskName);
	CheckExistedFlash(m_nSignature, m_nMaskCS, m_Flash);
}

void CFlashReaderDoc::DisableAllMode()
{
	memset(m_nSubTaskState, StateDisable, ModeCount*MAX_CS);
}

void CFlashReaderDoc::SetMode(BYTE bModeFrom, BYTE bModeTo, BYTE nState)
{
	for (BYTE nMode = bModeFrom; nMode <= bModeTo; nMode++)
	{
		for (BYTE nCS = 0; nCS < m_Flash.m_nCountCS; nCS++) m_nSubTaskState[nMode*MAX_CS + nCS] = nState;
	}
}

void CFlashReaderDoc::SetNewTaskMode(BYTE nNewMode)
{
	BOOL bRemove = (nNewMode < m_nTaskMode);
	SetMode((bRemove ? nNewMode : m_nTaskMode) + 1, bRemove ? m_nTaskMode : nNewMode, bRemove ? StateDisable : StateWait);
	m_nTaskMode = nNewMode;
}

void CFlashReaderDoc::OnDatabaseEdit()
{
	CBaseEditDlg	dlg(m_Database);
	dlg.DoModal();
}

void CFlashReaderDoc::OnDatabaseImport()
{
#ifdef USER_MODE
	return;
#endif
	CString			strExtantion(EXTANSION_IMPORT);
	CString			strMask(_T("*")EXTANSION_IMPORT);
	CString			strFilters;
	CFlashDatabase	dbSrc;

	strExtantion.Remove(_T('.'));
	strFilters.Format(IDS_PROMT_IMPORT, strMask, strMask);
	CFileDialog fileDlg(TRUE, strExtantion, strMask, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, strFilters);
	if(fileDlg.DoModal() != IDOK) return;
	BeginWaitCursor();
	dbSrc.Import(fileDlg.GetPathName());
	m_Database.Update(dbSrc);
	EndWaitCursor();
}

void CFlashReaderDoc::OnDatabaseMerge()
{
	CString			strExtantion(EXTANSION_DATABASE);
	CString			strMask(_T("*")EXTANSION_DATABASE);
	CString			strFilters;
	CFlashDatabase	dbSrc;

	strExtantion.Remove(_T('.'));
	strFilters.Format(IDS_PROMT_MERGE, strMask, strMask);
	CFileDialog fileDlg(TRUE, strExtantion, strMask, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, strFilters);
	if(fileDlg.DoModal() != IDOK) return;
	BeginWaitCursor();
	dbSrc.Load(fileDlg.GetPathName());
	m_Database.Update(dbSrc);
	EndWaitCursor();
}

CString CFlashReaderDoc::GetAppFileName()
{
	TCHAR szFileName[MAX_PATH];
	DWORD cch = GetModuleFileName(NULL, szFileName, MAX_PATH);	
	ASSERT(cch != 0);
	return CString(szFileName);
}

CString CFlashReaderDoc::GetAppDirectory()
{
	CString	strWorkDirectory(GetAppFileName());
	PathRemoveFileSpec(strWorkDirectory.GetBuffer());
	strWorkDirectory.ReleaseBuffer();
	return strWorkDirectory;
}

CString CFlashReaderDoc::GetDataFileName()
{
	CString	strDataFileName(m_strTaskName);
	PathRenameExtension(strDataFileName.GetBuffer(), EXTANSION_DATA);
	strDataFileName.ReleaseBuffer();
	return strDataFileName;
}

CString CFlashReaderDoc::GetDataFileName(BYTE nCS)
{
	CString	strDataFileName = GetDataFileName();
	CString	strCS;
	strCS.Format(_T("_%d"), nCS);
	strDataFileName.Insert(strDataFileName.GetLength()-4, strCS);
	return strDataFileName;
}

CString	CFlashReaderDoc::GetDatabaseName()
{
	CString	strDatabaseName(GetAppFileName());
	PathRenameExtension(strDatabaseName.GetBuffer(), EXTANSION_DATABASE);
	strDatabaseName.ReleaseBuffer();
	return strDatabaseName;
}

CString CFlashReaderDoc::GetLogFileName()
{
	CString	strLogFileName = GetAppFileName();
	PathRenameExtension(strLogFileName.GetBuffer(), EXTANSION_LOG);
	strLogFileName.ReleaseBuffer();
	return strLogFileName;
}

BOOL CFlashReaderDoc::IsConnected()
{
	return (BOOL)m_pReader;
}

BOOL CFlashReaderDoc::IsRunning()
{
	return ((BOOL)m_pReader && m_nMaskCS && m_bStarted);
}

void CFlashReaderDoc::StartNext()
{
	m_bStarted = FALSE;
	for (BYTE nMode = ModeFirstRead; nMode <= m_nTaskMode; nMode++)
	{
		for (BYTE nCS = 0; nCS < m_Flash.m_nCountCS; nCS++)
		{
			if ((m_nSubTaskState[nMode * MAX_CS + nCS] < StateComplete) && (BIT(nCS) & m_nMaskCS))
			{
				m_nSubTaskState[nMode * MAX_CS + nCS] = StateWork;
				m_nCurrentMode = nMode;
				m_nCurrentCS = nCS;
				m_bStarted = TRUE;
				m_threadRead.Start(m_nCurrentMode, m_nCurrentCS, m_bSeparateCS ? GetDataFileName(nCS) : GetDataFileName());
				return;
			}
		}
	}
}

void CFlashReaderDoc::UpdateTask(BYTE nPageStateNew)
{
	switch (nPageStateNew)
	{
	case PageStateNo:		SetMode(ModeFirstRead, m_nTaskMode, StateWait); break;
	case PageStateOnce:		SetMode(ModeVerify, m_nTaskMode, StateWait); break;
	case PageStateVerErr:	SetMode(ModeCorrect, m_nTaskMode, StateWait); break;
	}
	DoSave(m_strTaskName);
	UpdateAllViews(NULL, UpdateInitial);
}

void CFlashReaderDoc::ChangePageState(BYTE nStateOld, BYTE nStateNew)
{
	m_FlashMap.ChangePageState(nStateOld, nStateNew);
	UpdateTask(nStateNew);
}

void CFlashReaderDoc::ChangePageState(UINT nPageFrom, UINT nPageTo, BYTE nStateNew)
{
	m_FlashMap.ChangePageState(nPageFrom, nPageTo, nStateNew);
	UpdateTask(nStateNew);
}

void CFlashReaderDoc::UpdateLogString(CString strLog)
{
	CString	strOut;
	CTime	timeLog = CTime::GetCurrentTime();
	strOut.Format(_T("%s - %s"), timeLog.Format(_T("%d/%m/%y, %X")), strLog);
	UpdateAllViews(NULL, UpdateLog, (CObject*)&strOut);
	StringToLogFile(strOut);
}

void CFlashReaderDoc::StringToLogFile(CString strLog)
{
	if (!m_bLogToFile) return;
	CStringA	strASCI(strLog);
	int			nInd = 0;
	while(true)
	{
		nInd = strASCI.Find("\n", nInd);
		if (nInd == -1) break;
		strASCI.Insert(nInd, "\r");
		nInd += 2;
	}
	m_fileLog.Write(strASCI, strASCI.GetLength());
}

void CFlashReaderDoc::OpenLogFile()
{
	if ((m_fileLog.m_hFile != INVALID_HANDLE_VALUE) || !m_bLogToFile) return;
	CTime		timeCurrent = CTime::GetCurrentTime();
	CString		strFileName = GetLogFileName();
	UINT		nOpenFlafs = CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite;
	CFileStatus	fileStatus;

	if (CFile::GetStatus(strFileName, fileStatus))
	{
		if ( fileStatus.m_size > ((ULONGLONG)m_nLogFileMaxLengthInMB * 1048576) )
		{	// Backup Log file, create new
			CString	strFileNameNew;
			strFileNameNew.Format(_T("%s\\%s%s"), GetAppDirectory(), timeCurrent.Format(_T("%y_%m_%d_%H_%M")), EXTANSION_LOG);
			CFile::Rename(strFileName, strFileNameNew);
		}
		else
		{	// Continue Log file
			nOpenFlafs |= CFile::modeNoTruncate;
		}
	}
	if (m_fileLog.Open(strFileName, nOpenFlafs, NULL))
	{
		m_fileLog.SeekToEnd();
	}
	else
	{
		m_bLogToFile = FALSE;
		AfxMessageBox(IDS_ERROR_LOG_FILE,MB_ICONEXCLAMATION);
	}
}

void CFlashReaderDoc::CloseLogFile()
{
	if (m_fileLog.m_hFile != INVALID_HANDLE_VALUE)
	{
		m_fileLog.Flush();
		m_fileLog.Close();
	}
}

void CFlashReaderDoc::ConnectReader(BOOL bShowDialog)
{
	COXInstanceManager&	instanceManager(theApp.m_InstanceManager);
	CSingleLock			dataLock(&instanceManager.GetAdditionalDataMutex());
	CString				strTemp;

	if (!dataLock.Lock(100))
	{
		strTemp.LoadString(IDS_CONNECT_BUSY);
		AfxMessageBox(strTemp);
		return;
	}
	CStringList			listSerialNumber;
	CReaderList			listReader;
	CString				strCurrent;
	CReader*			pReader;
	DWORD				nDataSize = __min(instanceManager.GetAdditionalDataSize(), SHARED_DATA_SIZE);
	TCHAR*				pData = (TCHAR*)instanceManager.GetAdditionalData();
	TCHAR				nReaderCount = *pData;

	if (m_pReader)
	{
		strCurrent = m_pReader->GetSerialNumber();
		delete m_pReader;
		m_pReader = NULL;
	}
	pData++;
	for (TCHAR i = 0; i < __min(nReaderCount, MAX_READER_COUNT); i++)
	{
		strTemp = pData;
		if (strTemp != strCurrent) listSerialNumber.AddTail(strTemp);
		pData += (strTemp.GetLength()+1);
	}
	for (int nDrive = 0; nDrive < MAX_PHYSICAL_DRIVE; nDrive++)
	{
		pReader = new CReader;
		strTemp.Format(_T("\\\\.\\PhysicalDrive%d"), nDrive);
		if (!pReader->Open(strTemp))
		{
			delete pReader;
			continue;
		}
		if (NULL != listSerialNumber.Find(pReader->GetSerialNumber()))
		{
			pReader->m_nState = ReaderBusy;
		}
		listReader.AddTail(pReader);
		if (listReader.GetCount() == MAX_READER_COUNT) break;
	}
	if (bShowDialog)
	{
		CSelectReaderDlg	dlg;
		dlg.m_pListReader = &listReader;
		if (IDOK == dlg.DoModal())
		{
			m_pReader = dlg.m_pReader;
		}
	}
	else
	{
		for (POSITION pos = listReader.GetHeadPosition(); pos != NULL; )
		{
			pReader = listReader.GetNext(pos);
			if (pReader->m_nState == ReaderFree)
			{
				m_pReader = pReader;
				break;
			}
		}
	}
	if (m_pReader) listSerialNumber.AddTail(m_pReader->GetSerialNumber());
	for (POSITION pos = listReader.GetHeadPosition(); pos != NULL; )
	{
		pReader = listReader.GetNext(pos);
		if (pReader != m_pReader) delete pReader;
	}
	listReader.RemoveAll();
	pData = (TCHAR*)instanceManager.GetAdditionalData();
	*pData = (TCHAR)listSerialNumber.GetCount();
	pData++;
	for (POSITION pos = listSerialNumber.GetHeadPosition(); pos != NULL; )
	{
		strTemp = listSerialNumber.GetNext(pos);
		_tcscpy_s(pData, 200, strTemp.GetBuffer());
		pData += (strTemp.GetLength()+1);
	}
	listSerialNumber.RemoveAll();
	dataLock.Unlock();
	m_pReader ? strTemp.Format(_T("%s (%s), S/N %s - %s"), m_pReader->GetModel(), m_pReader->GetFirmware(), m_pReader->GetSerialNumber(), m_pReader->GetCurrentMode()) : strTemp.Empty();
	m_pFrameWnd->ShowReaderInfo(strTemp);
	if (m_Flash.IsEnable()) CheckExistedFlash(m_nSignature, m_nMaskCS, m_Flash);
}

void CFlashReaderDoc::OnReaderConnect()
{
	ConnectReader();
}

void CFlashReaderDoc::OnReaderStart()
{
	CString	strLog;
	if (!CheckExistedFlash(m_nSignature, m_nMaskCS, m_Flash)) return;
	if (!m_threadRead.InitThread(m_pFrameWnd->m_hWnd, m_pReader, m_Flash, m_nLimit, m_nDelta, m_nStepCount, m_bSeparateCS)) return;
	StartNext();
}

void CFlashReaderDoc::OnReaderStop()
{
	m_threadRead.Stop();
}

void CFlashReaderDoc::OnReaderSettings()
{
	CSettingsDlg	dlg;
	dlg.m_strDirectory = m_strDataDirectoty;
	dlg.m_nLimit = m_nLimit;
	dlg.m_nDelta = m_nDelta;
	dlg.m_nStepCount = m_nStepCount;
	dlg.m_bSeparateCS = (BOOL)m_bSeparateCS;
	dlg.m_bLogEnable = m_bLogToFile;
	dlg.m_nLogSize = m_nLogFileMaxLengthInMB;
	if (IDOK == dlg.DoModal())
	{
		m_strDataDirectoty = dlg.m_strDirectory;
		theApp.WriteProfileString(PROFILE_SETINGS, PROFILE_DATAPATH, m_strDataDirectoty);
		m_nLimit = dlg.m_nLimit;
		theApp.WriteProfileInt(PROFILE_SETINGS, PROFILE_CORRECTION_LIMIT, m_nLimit);
		m_nDelta = dlg.m_nDelta;
		theApp.WriteProfileInt(PROFILE_SETINGS, PROFILE_CORRECTION_DELTA, m_nDelta);
		m_nStepCount = dlg.m_nStepCount;
		theApp.WriteProfileInt(PROFILE_SETINGS, PROFILE_CORRECTION_STEP, m_nStepCount);
		m_bSeparateCS = (BYTE)dlg.m_bSeparateCS;
		theApp.WriteProfileInt(PROFILE_SETINGS, PROFILE_SEPARATE_CS, m_bSeparateCS);
		m_bLogToFile = dlg.m_bLogEnable;
		theApp.WriteProfileInt(PROFILE_SETINGS, PROFILE_LOG_ENABLE, m_bLogToFile);
		m_nLogFileMaxLengthInMB = dlg.m_nLogSize;
		theApp.WriteProfileInt(PROFILE_SETINGS, PROFILE_LOG_LENGTH, m_nLogFileMaxLengthInMB);
		m_bLogToFile ? OpenLogFile() : CloseLogFile();
	}
}

void CFlashReaderDoc::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

void CFlashReaderDoc::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

void CFlashReaderDoc::OnUpdateReaderConnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

void CFlashReaderDoc::OnUpdateReaderStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsConnected() && !m_bStarted/* && m_nMaskCS*/);
}

void CFlashReaderDoc::OnUpdateReaderStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bStarted);
}

void CFlashReaderDoc::OnUpdateReaderSettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

void CFlashReaderDoc::OnUpdateDatabaseEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

void CFlashReaderDoc::OnUpdateDatabaseImport(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}

void CFlashReaderDoc::OnUpdateDatabaseMerge(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bStarted);
}
