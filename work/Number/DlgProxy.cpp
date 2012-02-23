// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Number.h"
#include "DlgProxy.h"
#include "NumberDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNumberDlgAutoProxy

IMPLEMENT_DYNCREATE(CNumberDlgAutoProxy, CCmdTarget)

CNumberDlgAutoProxy::CNumberDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CNumberDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CNumberDlg)))
		{
			m_pDialog = reinterpret_cast<CNumberDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CNumberDlgAutoProxy::~CNumberDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CNumberDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CNumberDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CNumberDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_INumber to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {27ED1D89-F8CF-4A66-86DE-75307C3B2C00}
static const IID IID_INumber =
{ 0x27ED1D89, 0xF8CF, 0x4A66, { 0x86, 0xDE, 0x75, 0x30, 0x7C, 0x3B, 0x2C, 0x0 } };

BEGIN_INTERFACE_MAP(CNumberDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CNumberDlgAutoProxy, IID_INumber, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {A47A38F5-D31D-49AC-B4BE-6070C4444757}
IMPLEMENT_OLECREATE2(CNumberDlgAutoProxy, "Number.Application", 0xa47a38f5, 0xd31d, 0x49ac, 0xb4, 0xbe, 0x60, 0x70, 0xc4, 0x44, 0x47, 0x57)


// CNumberDlgAutoProxy message handlers
