#include "stdafx.h"
#include "FlashReader.h"
#include "MainFrm.h"
#include "FlashReaderDoc.h"
#include "FlashReaderView.h"
#include "AboutDlg.h"
#include "DumpEdit\DumpEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFlashReaderApp theApp;

BEGIN_MESSAGE_MAP(CFlashReaderApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFlashReaderApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

CFlashReaderApp::CFlashReaderApp()
: m_InstanceManager(_T("FlashReader"), SHARED_DATA_SIZE)
{
	m_InstanceManager.SetMaxAllowedInstances(MAX_READER_COUNT, FALSE);
}

BOOL CFlashReaderApp::InitInstance()
{
	CWinApp::InitInstance();

	if (((int)::GetVersion()) < 0)
	{
		AfxMessageBox(IDS_WRONG_WINDOWS, MB_ICONSTOP);
		return FALSE;
	}
	DWORD	nVolumeSerialNumber;
	DWORD	nMaximumComponentLength;
	DWORD	nFileSystemFlags;
	CString strVolume;
	CString strFileSystem;
	GetVolumeInformation(NULL, strVolume.GetBuffer(MAX_PATH+1), MAX_PATH+1, &nVolumeSerialNumber, &nMaximumComponentLength, &nFileSystemFlags, strFileSystem.GetBuffer(MAX_PATH+1), MAX_PATH+1);
	strVolume.ReleaseBuffer();
	strFileSystem.ReleaseBuffer();
	if (strFileSystem != _T("NTFS"))
	{
		AfxMessageBox(IDS_WRONG_FILE_SYSTEM, MB_ICONSTOP);
		return FALSE;
	}
		
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxInitRichEdit2();

	CDumpEdit::RegisterClass();

	SetRegistryKey(_T("Epos LTD"));
	LoadStdProfileSettings(10);
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME, RUNTIME_CLASS(CFlashReaderDoc), RUNTIME_CLASS(CMainFrame), RUNTIME_CLASS(CFlashReaderView));
	if (!pDocTemplate) return FALSE;
	AddDocTemplate(pDocTemplate);
	
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (!ProcessShellCommand(cmdInfo)) return FALSE;

	CRect rectTemp;
	m_pMainWnd->GetWindowRect(rectTemp);
	((CMainFrame*)m_pMainWnd)->SetMinTrackSize(CSize(rectTemp.Width(), rectTemp.Height()));
	m_pMainWnd->CenterWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

void CFlashReaderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
