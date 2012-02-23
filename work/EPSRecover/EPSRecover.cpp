#include "stdafx.h"
#include "EPSFile.h"

void Run(FileEx &in_file, TCHAR *out_dir);
BOOL SaveFilePartToAnotherFile(File *src_file, File *dst_file, LONGLONG offset, LONGLONG size);

void PrintUsage(void)
{
	_tprintf(_T("USAGE: EPSRecover.exe <in_file> <out_dir>\n\n") \
			 _T("<in_file> can be simple file or physical drive(\\\\.\\PhysicalDriveXXX).\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
	//if (argc < 3)
	//{
	//	PrintUsage();
	//}

	//=================================================================

	//TCHAR *in_file_name = argv[2];
	//TCHAR *out_dir_name = argv[3];

	FileEx in_file(argv[1]);
	TCHAR *out_dir = argv[2];

	Run(in_file, out_dir);


	//=================================================================

	_tprintf(_T("\n\nDone\n\n"));
	_tprintf(_T("Press any key ..."));
	_getch();
	return 0;
}

BOOL SaveFilePartToAnotherFile(File *src_file, File *dst_file, LONGLONG offset, LONGLONG size)
{
	if (!src_file || !dst_file || !size)
	{
		return FALSE;
	}

	if (!src_file->IsOpen() || !dst_file->IsOpen())
	{
		return FALSE;
	}

	DWORD rw = 0;
	BYTE rw_buff[512] = {0};
	LONGLONG sector_count = (LONGLONG)size/512;
	DWORD tail = (DWORD)size%512;

	src_file->SetPointer(offset);
	for (int i = 0; i < sector_count; ++i)
	{
		rw = src_file->Read(rw_buff, 512);
		dst_file->Write(rw_buff, rw);
	}

	if (tail)
	{
		src_file->Read(rw_buff, 512);
		dst_file->Write(rw_buff, tail);
	}

	src_file->SetPointer(offset);

	return TRUE;
}

void Run(FileEx &in_file, TCHAR *out_dir)
{
	if (!in_file.IsOpen())
	{
		if (!in_file.Open(GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE))
		{
			_tprintf(_T("Error in FileEx::OpenFile(): %s"), in_file.GetName());
			return;
		}
	}
	
	DWORD rw = 0;
	BYTE rw_buffer[512];
	LONGLONG file_ptr = 0;
	PEPSHEADER header;
	DWORD file_counter = 0;
	TCHAR file_name[MAX_PATH] = {0};

	//file_ptr = 1556391*512;
	//in_file.SetPointer(file_ptr);

	while ( (rw = in_file.Read(rw_buffer, 512)) == 512 )
	{
		header = (PEPSHEADER)rw_buffer;
		if (EPSFile::IsHeader(header))
		{
			_tprintf(_T("#%010d: Found file header\n"), file_ptr/512);

			// Формируем первое имя файла
			_stprintf_s(file_name, MAX_PATH, _T("%s\\EPS#%06d.eps"), out_dir, file_counter);

			EPSFile eps_file(file_name, header);
			if (eps_file.Create())
			{
				// Сохраняем тело файла в EPS файл
				SaveFilePartToAnotherFile(&in_file, &eps_file, file_ptr, eps_file.GetSize());

				// Восстанавливаем файловый указатель на сектор дальше чем найденный EPSHEADER
				in_file.SetPointer(file_ptr + 512);
			}
			++file_counter;
		}
		file_ptr += rw;
	}
	return;
}