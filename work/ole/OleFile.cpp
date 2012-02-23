#include "stdafx.h"
#include "OleFile.h"

TCHAR *OleFile::GetFileType(void)
{
	return NULL;
}

TCHAR *OleFile::GetExtension(void)
{
	return NULL;
}

DWORD OleFile::GetMinSize(void)
{
	return 0;
}

DWORD OleFile::GetMaxSize(void)
{
	return 0;
}

BOOL OleFile::IsBeginOfFile(BYTE *buffer, DWORD size)
{
	return TRUE;
}