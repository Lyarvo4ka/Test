#include "stdafx.h"
#include <conio.h>
#include "W32Lib.h"

using namespace W32Lib;

#define MAX_STR_LEN		255

BOOL IsMPEG(BYTE *buff)
{
	if (buff[0] == 0x00)
	if (buff[1] == 0x00)
	if (buff[2] == 0x01)
	if (buff[3] == 0xBA)
		return TRUE;

	return FALSE;
}

BOOL CheckDirPath(TCHAR *dir)
{
	DWORD length = _tcslen(dir);
	if (length && (length + 2 < MAX_STR_LEN))
	{
		TCHAR ch = dir[length - 1];
		if (ch != _T('\\'))
		{
			dir[length] = _T('\\');
			dir[length+1] = _T('\0');
		}
		return TRUE;
	}
	return FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		_tprintf(_T("\nUsage: <input_file> <output_dir>"));
		_getch();
		return 0;
	}
	else
	{
		; 
	}

	_tprintf(_T("\n Done. Press any key ..."));
	_getch();

	return 0;
}

