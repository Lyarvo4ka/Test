//#include "StdAfx.h"
//#include "RawFilesMgr.h"
//
//RawFilesMgr::RawFilesMgr(void)
//{
//}
//
//RawFilesMgr::~RawFilesMgr(void)
//{
//}
//
//BOOL RawFilesMgr::UpdateRawFilesList(void)
//{
//	CString findDirectoryPath;
//	findDirectoryPath = pluginsDirPath + _T("\\*.dll");
//
//	WIN32_FIND_DATA find_data;
//	memset(&find_data, 0x00, sizeof(WIN32_FIND_DATA)); 
//	HANDLE hFind = FindFirstFile(findDirectoryPath.GetBuffer(), &find_data);
//	if (hFind != INVALID_HANDLE_VALUE)
//	{
//		CString pluginPath;
//		pluginPath = pluginsDirPath + _T("\\") + find_data.cFileName;
//		Plugin plugin(pluginPath);
//		if (plugin.Load())
//		{
//
//		}
//
//		int x = 0;
//	}
//	return TRUE;
//}
