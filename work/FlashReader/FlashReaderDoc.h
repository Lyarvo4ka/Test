#pragma once

#include "Reader.h"
#include "FlashDatabase.h"
#include "FlashRecord.h"
#include "FlashMap.h"
#include "ThreadRead.h"

enum SubTaskState
{
	StateWait,
	StateWork,
	StateComplete,
	StateFail,
	StateDisable
};

enum UpdateType
{
	UpdateInitial,
	UpdateCheck,
	UpdateStepStart,
	UpdateStepProgress,
	UpdateStepEnd,
	UpdateLog,
	UpdateLogClear
};

class CMainFrame;

class CFlashReaderDoc : public CDocument
{
public:
	CFlashDatabase		m_Database;
	CFlashRecord		m_Flash;
	CFlashMap			m_FlashMap;
	ULONGLONG			m_nSignature;
	ULONGLONG			m_nMaskID;
	BYTE				m_nCurrentCS;
	BYTE				m_nCurrentMode;
	BYTE				m_nTaskMode;
	BYTE				m_nMaskCS;
	BYTE				m_nSubTaskState[ModeCount*MAX_CS];

protected:
	CMainFrame*			m_pFrameWnd;
	CReader*			m_pReader;
	CThreadRead			m_threadRead;
	CFile				m_fileLog;
	CString				m_strTaskName;
	CString				m_strDataDirectoty;
	ULONG				m_nLogFileMaxLengthInMB;
	UINT				m_nLimit;
	UINT				m_nDelta;
	UINT				m_nStepCount;
	BOOL				m_bEnableNewTask;
	BOOL				m_bStarted;
	BOOL				m_bLogToFile;
	BYTE				m_bSeparateCS;

public:
	virtual ~CFlashReaderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void DeleteContents();
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	void OnReadThreadMessage(DWORD nMsg, ULONG nMsgData);
	void SetNewTaskMode(BYTE nNewMode);
	BOOL IsConnected();
	BOOL IsRunning();
	void ChangePageState(BYTE nStateOld, BYTE nStateNew);
	void ChangePageState(UINT nPageFrom, UINT nPageTo, BYTE nStateNew);
	BOOL CheckNewFlash(ULONGLONG& nSignature, BYTE& nMaskCS, CFlashRecord& flash);
	BOOL CheckExistedFlash(ULONGLONG& nSignature, BYTE& nMaskCS, CFlashRecord& flash);

protected:
	CFlashReaderDoc();
	afx_msg void OnReaderConnect();
	afx_msg void OnReaderStart();
	afx_msg void OnReaderStop();
	afx_msg void OnReaderSettings();
	afx_msg void OnDatabaseEdit();
	afx_msg void OnDatabaseImport();
	afx_msg void OnDatabaseMerge();
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateReaderConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateReaderStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReaderStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReaderSettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDatabaseEdit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDatabaseImport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDatabaseMerge(CCmdUI *pCmdUI);
	
private:
	CString GetAppFileName();
	CString GetAppDirectory();
	CString GetDataFileName();
	CString GetDataFileName(BYTE nCS);
	CString	GetDatabaseName();
	CString GetLogFileName();
	BOOL CheckFlash(ULONGLONG& nSignature, BYTE& nMaskCS, CFlashRecord& flash, BYTE* pBuf);
	BOOL ReadAdditionalParametersPages(CFlashRecord& flash, BYTE* pBuf);
	void ConnectReader(BOOL bShowDialog = TRUE);
	void StartNext();
	void InitTask(BOOL bNew = TRUE);
	void PrepareDataFile(CString strFileName, ULONGLONG nSize, BOOL bNew);
	void DisableAllMode();
	void SetMode(BYTE bModeFrom, BYTE bModeTo, BYTE nState);
	void UpdateTask(BYTE nPageStateNew);
	void UpdateLogString(CString strLog);
	void StringToLogFile(CString strLog);
	void OpenLogFile();
	void CloseLogFile();

	DECLARE_DYNCREATE(CFlashReaderDoc)
	DECLARE_MESSAGE_MAP()
};
