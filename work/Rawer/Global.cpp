#include "stdafx.h"
#include "Global.h"

namespace Global {

	const CString strMainWndClass = _T("RawerMainWnd");
	const CString strMainWndName = _T("Rawer");

	BOOL IsPathExists(TCHAR *path)
	{
		if (!path) return FALSE;

		WIN32_FIND_DATA find_data;
		memset(&find_data, 0x00, sizeof(WIN32_FIND_DATA));
		if (INVALID_HANDLE_VALUE == FindFirstFile(path, &find_data))
			return FALSE;
		else
			return TRUE;
	}
}



