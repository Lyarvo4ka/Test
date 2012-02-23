#include "Function.h"

using namespace W32Lib;

BOOL W32Lib::ForEachFileFolder(LPCTSTR path,
							   FOREACHFILEPROC file_proc,
							   FOREACHFILEPROC folder_proc,
							   BOOL check_subfolders)
{
	if (!path || !(file_proc || folder_proc))
		return FALSE;

	String find_path = path;
	find_path += _T("\\*.*");

	WIN32_FIND_DATA wfd = {0};
	HANDLE hFind = ::FindFirstFile(find_path.data(), &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			String file_name = wfd.cFileName;

			if ( (file_name == _T(".")) || (file_name == _T("..")) )
				continue;

			String full_file_path = path;
			full_file_path += _T("\\");
			full_file_path += file_name;

			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (check_subfolders)
					ForEachFileFolder(full_file_path.c_str(), file_proc, folder_proc, check_subfolders);
				if (folder_proc)
					folder_proc((TCHAR *)full_file_path.data());
			}
			else if (file_proc)
				file_proc((TCHAR *)full_file_path.data());


		} while (::FindNextFile(hFind, &wfd));

		::FindClose(hFind);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL W32Lib::CmpMemBlck(BYTE *blck1, BYTE *blck2, DWORD size)
{
	if (!blck1 || !blck2 || !size) return FALSE;
	for (DWORD i = 0; i < size; ++i) {
		if (blck1[i] != blck2[i])
			return FALSE;
	}
	return TRUE;
}

String W32Lib::GetFileNameFromFullPathName(const String &full_path_name)
{
	DWORD name_len = 0;
	DWORD path_len = full_path_name.length();
	while (path_len)
	{
		path_len--;
		if (full_path_name[path_len] == _T('\\'))
		{
			return full_path_name.substr(path_len + 1, name_len);
		}
		else
		{
			++name_len;
		}
	}
	return String(_T(""));
}

String W32Lib::GetFileExtensionFromFileName(const String &file_name)
{
	DWORD ext_len = 0;
	DWORD name_len = file_name.length();
	while (name_len)
	{
		name_len--;
		if (file_name[name_len] == _T('.'))
		{
			return file_name.substr(name_len + 1, ext_len);
		}
		else
		{
			ext_len++;
		}
	}
	return String(_T(""));
}