#include "stdafx.h"
#include "MainDlg.h"

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	ON_WM_DEVMODECHANGE()
END_MESSAGE_MAP()

void MainDlg::OnDevModeChange(LPTSTR lpDeviceName)
{
	CDialog::OnDevModeChange(lpDeviceName);

	// TODO: Add your message handler code here
}
