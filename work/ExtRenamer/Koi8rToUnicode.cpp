#include "stdafx.h"
#include "Koi8rToUnicode.h"

void Koi8rToUnicode(String &koi8r_str, String &unicode_str)
{
	CHAR *ansi_str_buff = NULL;
	int ret = 0;
	
	if (!(ret = WideCharToMultiByte(65001, 0, koi8r_str.c_str(), koi8r_str.length(), (LPSTR)NULL, 0, NULL, NULL)))
		return;
	++ret;
	ansi_str_buff = new CHAR[ret];
	memset(ansi_str_buff, 0x00, ret);

	if (!(ret = WideCharToMultiByte(65001, 0, koi8r_str.c_str(), koi8r_str.length(), ansi_str_buff, ret, NULL, NULL)))
		return;

	DWORD i = 0;	
}

DWORD WINAPI ChangeCpInFileFolderName(TCHAR *name)
{
	String old_name = name;
	String new_name;

	Koi8rToUnicode(old_name, new_name);

	return 0;
}