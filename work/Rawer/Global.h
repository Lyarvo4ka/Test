#pragma once
#include "Engine.h"

namespace Global
{
	////////////////////////////////////////////////////////
	//	Global variables
	//

	extern const CString strMainWndClass;
	extern const CString strMainWndName;

	////////////////////////////////////////////////////////
	//	Global function
	//

	// Прверяет существует ли указанный путь (может быть папка либо файл)
	inline BOOL IsPathExists(TCHAR *path);
};

