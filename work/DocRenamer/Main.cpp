#include "stdafx.h"
#include "Main.h"
#include "MainWnd.h"

ULONGLONG file_counter = 0;
ULONGLONG folder_counter = 0;

DWORD CALLBACK ForFilesProc(TCHAR *full_path)
{
	HRESULT res;
	IStorage *root_storage = NULL;
	IPropertySetStorage *property_set_storage = NULL;
	ULONGLONG size = 0;

	res = StgOpenStorage(full_path, NULL, STGM_DIRECT|STGM_READ|STGM_SHARE_DENY_WRITE, NULL, 0, &root_storage);
	if (res == S_OK)
	{
		IEnumSTATSTG *enum_stat_stg = NULL;
		res = root_storage->EnumElements(0, NULL, 0, &enum_stat_stg);
		if (res == S_OK)
		{
			STATSTG stat_stg = {0};
			ULONG ret_element = 0;
			while (S_OK == enum_stat_stg->Next(1, &stat_stg, &ret_element))
			{
				_tprintf(L"%s\n", stat_stg.pwcsName);
				if (stat_stg.type == STGTY_STREAM)
					/*if (0 == _tcscmp(stat_stg.pwcsName, L"WordDocument"))*/
				{
					IStream *stream = NULL; 
					res = root_storage->OpenStream(stat_stg.pwcsName, 0, STGM_DIRECT|STGM_READ|STGM_SHARE_EXCLUSIVE, 0, &stream);
					if (res == S_OK)
					{
						BYTE buffer[1024] = {0};
						res = stream->Read(buffer, 1024, &ret_element);
						if (res == S_OK)
						{
							_tprintf(L"%s\n", buffer);
							int x = 0;
						}
						stream->Release();
					}
				}
				size += stat_stg.cbSize.QuadPart;
				CoTaskMemFree( stat_stg.pwcsName );
				stat_stg.pwcsName = NULL;
			}
			enum_stat_stg->Release();	
		}
		root_storage->Release();
	}
	return 0;
}

DWORD CALLBACK ForFoldersProc(TCHAR *full_path)
{
	++folder_counter;
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	try
	{
		W32App app;
		MainWnd main_wnd;
		main_wnd.Create(L"Doc renamer", WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX, 0, 0, 400, 200, NULL);
		main_wnd.Show();

		return app.Run();
	}
	catch(Exception &e)
	{
		e.ShowMessage();
		return ::GetLastError();
	}
}



