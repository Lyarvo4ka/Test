#include "stdafx.h"
#include "SonyMTSRecovery.h"

String MakeNextFileName(DWORD file_num, const String &folder_name)
{
	String new_file_name;
	DWORD folder_name_len = folder_name.size();
	if (folder_name_len < MAX_PATH)
	{
		TCHAR str[MAX_PATH] = {0};
		_stprintf_s(str, MAX_PATH, _T("%s\\%s%04d.%s"), folder_name.c_str(), MTS_FILE_NAME, file_num, MTS_FILE_EXT);
		new_file_name = str;
	}
	return new_file_name;
}

DWORD Run(const String &in_file_name, const String &out_folder_name)
{
	FileEx in_file(in_file_name.c_str());
	FileEx *out_file = NULL;
	DWORD file_counter = 0;
	DWORD file_size = 0;
	if (in_file.Open())
	{
		DWORD rw = 0;
		BYTE rw_buff[RW_BUFFER_SIZE] = {0};
		BYTE mts_sign_buff[] = {0x47,0x40,0x00,0x10,0x00,0x00,0xB0,0x11,0x00,0x00,0xC1,0x00,0x00,0x00,0x00,0xE0,0x1F,0x00,0x01,0xE1,0x00,0x23,0x5A,0xAB,0x82};
		Signature mts_sign(mts_sign_buff, 25);
		while ((rw = in_file.Read(rw_buff, RW_BUFFER_SIZE)) >= 29)
		{
			Signature curr_sign(&rw_buff[4], mts_sign.Count());
			if (curr_sign == mts_sign)
			{
				if (out_file)
				{
					delete out_file;
					out_file = NULL;
					file_size = 0;
				}

				out_file = new FileEx(MakeNextFileName(++file_counter, out_folder_name.c_str()).c_str());
				if (out_file->Create())
				{
					file_size += out_file->Write(rw_buff, rw);
				}
				else
				{
					delete out_file;
					out_file = NULL;
					file_size = 0;
				}
			}
			else
			{
				if (out_file)
				{
					file_size += out_file->Write(rw_buff, rw);
				}
			}
		}
	}
	if (out_file)
	{
		delete out_file;
	}
	return (DWORD)(-1);
}