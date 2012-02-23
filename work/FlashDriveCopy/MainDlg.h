#pragma once

#include "Resource.h"

class MainDlg : public CDialog
{
private:

public:
	MainDlg() : CDialog(IDD_MAIN_DIALOG) {}
	~MainDlg() {}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
};

