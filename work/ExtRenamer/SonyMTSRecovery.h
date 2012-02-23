#ifndef _SONYMTSRECOVERY
#define _SONYMTSRECOVERY

#include "kernel.h"

#define MTS_FILE_NAME				_T("Video")
#define MTS_FILE_EXT				_T("mts")
#define RW_BUFFER_SIZE				(DWORD)(64*512)

String MakeNextFileName(DWORD file_num, const String &folder_name);
DWORD Run(const String &in_file_name, const String &out_folder_name);

#endif // _SONYMTSRECOVERY