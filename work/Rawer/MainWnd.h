#pragma once
#include "RawerApp.h"
#include "Global.h"
#include "Box.h"
//#include "GdiPlus.h"

//using namespace Gdiplus;

#define	BORDER		5

class MainWnd : public CWnd
{
		DECLARE_DYNAMIC(MainWnd)
	private:

		enum {DRIVE_CB_ID = 1};

		CFont *font;
		CStatic *source_box;
		CStatic *file_type_box;
		CComboBox *drive_cb;

		CBox *tstBox;
		CBox *tstBox2;

		BOOL InitializeWnd(void);
		BOOL CreateControls(void);
		void ReallocateControls(void);

		afx_msg void OnDriveCbSelEndOk();
		afx_msg void OnPaint();

	public:

		MainWnd();
		virtual ~MainWnd();
	
	protected:
		DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


