#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "OXInstanceManager.h"
#include "Shlwapi.h"

class CFlashReaderApp : public CWinApp
{
public:
	COXInstanceManager		m_InstanceManager;

protected:

public:
	CFlashReaderApp();
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();

protected:

	DECLARE_MESSAGE_MAP()
};

extern CFlashReaderApp theApp;