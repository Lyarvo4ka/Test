#include "stdafx.h"
#include <conio.h>
#include <W32Lib.h>
#include "TBBFile.h"

int RecoverEmlFiles(TCHAR *file_name, TCHAR *work_dir);

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc <= 2) {
		_tprintf(_T("Wrong input :( \nPress any key ...\n"));
		_getch();
		return 0; 
	}

	RecoverEmlFiles(argv[1], argv[2]);

	_tprintf(_T("\nDone. Press any key ..."));
	_getch();
	return 0;
}

BOOL SaveMessageToEmlFile(MESSAGE *msg, TCHAR *work_dir)
{
	static DWORD curr_dir_num		= 0;
	static DWORD files_per_dir		= 0;
	static TCHAR out_dir[MAX_PATH]	= {0};
	static TCHAR fname[MAX_PATH]	= {0};

	if (!msg || !work_dir)
		return FALSE;

	if (files_per_dir >= 5000) {
		files_per_dir = 0;
		curr_dir_num++;
		memset(out_dir, 0x00, MAX_PATH);
	} 

	if (!_tcslen(out_dir)) {
		_stprintf_s(out_dir, MAX_PATH, _T("%s\\DIR#%04d"), work_dir, curr_dir_num);
		if (!CreateDirectory(out_dir, NULL))
			if (ERROR_ALREADY_EXISTS != GetLastError())
				return FALSE;	
	}

	_stprintf_s(fname, MAX_PATH, _T("%s\\MSG#%06d.eml"), out_dir, files_per_dir);
	FileEx eml_file(fname);
	if (eml_file.Create()) {
		++files_per_dir;
		if (msg->header.message_size == eml_file.Write(msg->body, msg->header.message_size))
			return TRUE;
	}

	return FALSE;
}


int RecoverEmlFiles(TCHAR *file_name, TCHAR *work_dir)
{
	FileEx input_file(file_name);
	if (!input_file.Open())
		return -1;

	DWORD	msg_sign	= MESSAGE_SIGNATURE;
	DWORD	msg_counter = 0;
	DWORD	rw			= 0;
	DWORD	buffer_size = 100*1024*1024;
	BYTE	*buffer		= new BYTE[buffer_size];
	MESSAGE *message	= (MESSAGE *)buffer;
	LONGLONG offset		= 0;

	while((offset = input_file.Find((BYTE *)&msg_sign, sizeof(msg_sign))) != -1)
	{
		// Читаем заголовок письма
		if (sizeof(MESSAGE_HEADER) == input_file.Read(buffer, sizeof(MESSAGE_HEADER)))
		{
			// Проверяем корректность заголовка
			if (message->header.header_size == sizeof(MESSAGE_HEADER))
			{
				// Читаем тело письма
				if (message->header.message_size == input_file.Read(&message->body, message->header.message_size))
				{
					// Проверяем концовку письма. Каждое писмо должно заканчиваться 0x0A0D
					if ((message->body[message->header.message_size - 1] == 0x0A) &&
						(message->body[message->header.message_size - 2] == 0x0D))
					{
						_tprintf(_T("MSG offset: %lld\n"), offset);
						if (SaveMessageToEmlFile(message, work_dir))
							++msg_counter;
					}
				}
			}
		}
	}

	_tprintf(_T("Total: %d\n"), msg_counter);
	return 0;
}
