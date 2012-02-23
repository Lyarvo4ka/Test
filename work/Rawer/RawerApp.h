#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "MainWnd.h"
#include "Global.h"
#include "Engine.h"
#include "RawFilesMgr.h"
//#include "GdiPlus.h"


#define PLUGINS_DIRECTORY_NAME		_T("Plugins")

inline void RegisterWndClasses(void)
{
	WNDCLASS wc;
	HBRUSH background_brush;

	if (!(background_brush = ::GetSysColorBrush(22)))
		AfxThrowResourceException();

	memset(&wc, 0x00, sizeof(wc));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hInstance = ::AfxGetInstanceHandle();
	wc.hIcon = ::LoadIcon(NULL, IDI_ASTERISK);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = background_brush;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Global::strMainWndClass;


	if (!AfxRegisterClass(&wc))
		AfxThrowResourceException();
}

class CApp : public CWinApp
{
	private:
		
		CWnd *pMainWnd;

		PhysicalDriveMgr driveMgr;
		RawFilesMgr rawFilesMgr;
		Rawer rawer;

		CString appDirectoryPath;
		CString plaginsDirectoryPath;

		BOOL InitializeAppDirPath(void);
		BOOL InitializePluginsDirPath(void);

	public:

		CApp();
		CApp(LPCTSTR lpszAppName) : CWinApp(lpszAppName) {}
		virtual BOOL InitInstance();
		virtual int ExitInstance();

		const CString *GetAppDirectory(void) {return &appDirectoryPath;}
		const CString *GetPluginsDirectory(void) {return &plaginsDirectoryPath;}

		DECLARE_MESSAGE_MAP()
};

extern CApp theApp;



