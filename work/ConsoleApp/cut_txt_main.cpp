#include "cut_txt_main.h"
#include "W32Lib.h"

#define BLOCK_SIZE						(DWORD)512
#define CHAR_NL							(BYTE)0x0A
#define CHAR_CR							(BYTE)0x0D
#define CHAR_SEPARATOR					(BYTE)0x7C
#define CHAR_NUL						(BYTE)0x00


BOOL IsValidChar(BYTE &ch)
{
	if ( (ch == 0x0A) || (ch == 0x0D) || (ch == 0x7C) || ((ch >= 0x30) && (ch <= 0x39)) )
		return TRUE;
	else
		return FALSE;
}

typedef struct _STATISTIC {
	DWORD digit_cnt;				// '0'-'9'
	DWORD separator_cnt;			// '|'
	DWORD nl_cnt;					// '\n'
	DWORD cr_cnt;					// '\r'
	DWORD nul_cnt;					// 0x00
	DWORD ch_cnt;
	DWORD unknown_cnt;
	DWORD buff_sz;
} STATISTIC;

void AnalyzeBuffer(BYTE *buff, DWORD sz, STATISTIC &statistic)
{
	memset(&statistic, 0x00, sizeof(statistic));
	statistic.buff_sz = sz;
	for (DWORD i = 0; i < sz; i++)
	{
		if ( (buff[i] >= 0x30) && (buff[i] <= 0x39) )
			statistic.digit_cnt++;
		else if (buff[i] == CHAR_SEPARATOR)
			statistic.separator_cnt++;
		else if (buff[i] == CHAR_NL)
			statistic.nl_cnt++;
		else if (buff[i] == CHAR_CR)
			statistic.cr_cnt++;
		else if (buff[i] == CHAR_NUL)
		{
			statistic.nul_cnt++;
			continue;
		}
		else
		{
			statistic.unknown_cnt++;
			continue;
		}
		statistic.ch_cnt++;
	}
}

BOOL IsPartOfTextFile(STATISTIC &statistic)
{
	if ( (statistic.unknown_cnt == 0x00) && (statistic.ch_cnt) )
	{
		DWORD symbol_cnt = statistic.digit_cnt + statistic.separator_cnt;
		if (symbol_cnt >= 17)
		{
			if (statistic.separator_cnt)
				return TRUE;
			else
				return FALSE;
		}
		else 
			return TRUE;
	}
	else
		return FALSE;
}

DWORD GetSequenceLength(BYTE *buff, DWORD sz)
{
	DWORD len = 0;
	for (DWORD i = 0; i < sz; i++)
	{
		if (IsValidChar(buff[i]))
			len++;
		else return len;
	}
	return len;
}

void SaveToFile(const FileEx &src_file, LONGLONG &ptr)
{
	DWORD rw = 0;
	BYTE buff[BLOCK_SIZE] = {0};
	STATISTIC statistic;
	TCHAR file_name[1024] = {0};

	_stprintf_s(file_name, 1024, _T("%06I64d.txt"), ptr); strlen();

	FileEx out_file(file_name);
	if (const_cast<FileEx &>(src_file).SetPointer(ptr))
	{
		while (rw = const_cast<FileEx &>(src_file).Read(buff, BLOCK_SIZE))
		{
			AnalyzeBuffer(buff, rw, statistic);
			if (IsPartOfTextFile(statistic) && out_file.Create())
			{
				if (statistic.ch_cnt < rw)
				{
					out_file.Write(buff, GetSequenceLength(buff, rw));
					break;
				}
				else
					out_file.Write(buff, rw);	
			}
			else return;
		}
	}
}

int CutTxtMain(TCHAR *file_name)
{
	FileEx src_file(file_name);
	if (src_file.Open())
	{
		DWORD rw = 0;
		LONGLONG ptr = 0;
		BYTE r_buff[BLOCK_SIZE] = {0};
		STATISTIC statistic;

		while (rw = src_file.Read(r_buff, BLOCK_SIZE))
		{
			AnalyzeBuffer(r_buff, rw, statistic);
			if (IsPartOfTextFile(statistic))
			{
				SaveToFile(src_file, ptr);
			}

			src_file.GetPointer(&ptr);
			_tprintf(_T("LBA: %I64d\n"), ptr/BLOCK_SIZE);
		}

		return 0;
	}
	return -1;
}

DWORD WINAPI CorrectFileSize(TCHAR *full_path)
{
	FileEx file(full_path);
	if (file.Open(GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE))
	{
		DWORD rw = 0;
		size_t sz = (size_t)file.GetSize();
		BYTE *r_buff = new BYTE[sz];

		memset(r_buff, 0x00, sz);

		if (rw = file.Read(r_buff, sz))
		{
			LONGLONG real_sz = 0;
			for (DWORD i = 0; i < rw; i++)
			{
				if (r_buff[i] != 0x00)
					real_sz++;
				else break;
			}
			if (real_sz)
			{
				if (!file.SetFileSize(real_sz))
				{
					DWORD err = ::GetLastError();
					int x = 0;
				}
				return 0;
			}
			else return -1;
		}
	}
	return 0;
}


int CorrectFileSizeMain(TCHAR *dir)
{
	if (dir)
	{
		ForEachFileFolder(dir, CorrectFileSize, NULL, TRUE);
	}

	return 0;
}