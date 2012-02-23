// DlgProxy.h: header file
//

#pragma once

class CNumberDlg;


// CNumberDlgAutoProxy command target

class CNumberDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CNumberDlgAutoProxy)

	CNumberDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CNumberDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CNumberDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CNumberDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

