#ifndef _KOI8R_TO_UNICODE
#define _KOI8R_TO_UNICODE

#include "W32Lib.h"

#define CP_KOI8R				20866
#define CP_KOI8U				21866
#define CP_1251					1251

void Koi8rToUnicode(String &koi8r_str, String &unicode_str);
DWORD WINAPI ChangeCpInFileFolderName(TCHAR *name);

#endif // _KOI8R_TO_UNICODE