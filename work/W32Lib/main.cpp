//#include "Core\W32App.h"
//
//using namespace W32Lib;
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//	W32App app;
//
//	TCHAR ret_file_name[MAX_PATH] = {0};
//	TCHAR *filter = _T("All files\0*.*\0Word files\0*.doc;*.docx\0Excel files\0*.xls;*.xlsx\0\0");
//	WORD file_name_offset = 0;
//
//	OPENFILENAME of = {0};
//	of.lStructSize	= sizeof(OPENFILENAME);
//	of.hwndOwner	= NULL;
//	of.hInstance	= NULL;
//	of.lpstrFilter	= filter;
//	of.lpstrFile	= ret_file_name;
//	of.nMaxFile		= MAX_PATH;
//	of.lpstrInitialDir = _T("d:\\Work");
//	of.lpstrTitle	= _T("Testing Open File :)");
//	of.nFileOffset	= file_name_offset;
//
//	GetOpenFileName(&of);
//
//	return 0;
//}
//
//BOOL ForEachFile(const TCHAR *start_dir)
//{
//	WIN32_FIND_DATA fdata = {0};
//	HANDLE hfind;
//
//	if ( (hfind = FindFirstFile(start_dir, &fdata)) != INVALID_HANDLE_VALUE )
//	{
//
//		
//		FindClose(hfind);
//		return TRUE;
//	}
//	else
//		return FALSE;
//}
//
//void ErrorHandler(void)
//{
//	DWORD error = ::GetLastError();
//	LPVOID msg_buffer = NULL;
//
//	//if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//	//					NULL, error, MAKELANGID(), msg_buffer, 0, NULL) )
//
//
//}

