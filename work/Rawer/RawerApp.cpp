#include "stdafx.h"
#include "RawerApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CApp theApp(_T("RAWer :)"));

CApp::CApp()
{
	appDirectoryPath = _T("");
	plaginsDirectoryPath = _T("");
}

BOOL CApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	try
	{
		::RegisterWndClasses();

		InitializeAppDirPath();
		InitializePluginsDirPath();

		driveMgr.UpdateDriveList();
		rawFilesMgr.SetPluginsDirPath(plaginsDirectoryPath);
		rawFilesMgr.UpdateRawFilesList();

		File *file1 = new File();
		File *file2 = new File();

		rawer.List<File>::Add(file1);
		rawer.List<File>::Add(file2);

		rawer.Run();

		pMainWnd = (CWnd *)new MainWnd;
		m_pMainWnd = pMainWnd;
		m_pMainWnd->CenterWindow();
		m_pMainWnd->ShowWindow(SW_SHOW);
	}
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL CApp::InitializeAppDirPath(void)
{
	DWORD str_buffer_size = ::GetCurrentDirectory(0, 0);
	if (str_buffer_size)
		str_buffer_size = str_buffer_size*sizeof(TCHAR);
	else
		return FALSE;

	try
	{
		TCHAR *str_buffer = new TCHAR[str_buffer_size];
		memset(str_buffer, 0x00, str_buffer_size);

		if (GetCurrentDirectory(str_buffer_size, str_buffer))
		{
			appDirectoryPath = str_buffer;
			delete str_buffer;
			return TRUE;
		}
		else
		{
			delete str_buffer;
			return FALSE;
		}
	}
	catch (...)
	{
		return FALSE;
	}
}

BOOL CApp::InitializePluginsDirPath(void)
{
	if (appDirectoryPath != _T(""))
	{
		plaginsDirectoryPath = appDirectoryPath + _T("\\") + PLUGINS_DIRECTORY_NAME;
		return TRUE;
	}
	else
		return FALSE;
}

int CApp::ExitInstance()
{
	driveMgr.Clear();

	if (pMainWnd) 
		pMainWnd->DestroyWindow();

	delete pMainWnd;
	
	return CWinApp::ExitInstance();
}