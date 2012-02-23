#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "W32Lib.h"
#include "RTKernel.h"

class CRTApp : public CWinApp
{
public:
	CRTApp();
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CRTApp theApp;