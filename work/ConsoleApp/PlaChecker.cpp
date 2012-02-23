#include "PlaChecker.h"

//inline BOOL StrCmp(const char *first_str, const char *second_str, DWORD count)
//{
//	if (memcmp(first_str, second_str) == 0x00)
//}

inline BOOL PlaHeaderIsValid(PLA_HEADER *header)
{
	char first_name[37];
	char second_name[37];
	memcpy(first_name, header->name1, 37);
	memcpy(second_name, header->name2, 37);
	if (memcmp(header->signature, PLA_SIGNATURE, sizeof(header->signature)) == 0x00)
	{
		if (strcmp(first_name, second_name) == 0x00)
			return TRUE;
	}
	return FALSE;
}

inline BOOL FileListHeaderIsValid(FILE_LIST_HEADER *file_list_header)
{
	if (memcmp(file_list_header->name, FILE_LIST_HEADER_NAME, sizeof(file_list_header->name)) == 0x00)
		return TRUE;
	else
		return FALSE;
}

inline LONGLONG FileListIsValid(FILE_ENTRY *file_list, DWORD size)
{
	DWORD count = (DWORD)(size/sizeof(FILE_ENTRY));
	LONGLONG pla_file_size = PLA_HEADER_SIZE + FILE_LIST_HEADER_SIZE + size;
	for (DWORD i = 0; i < count; ++i)
	{
		if (file_list[i].unknown == 0x00)
		{
			if (file_list[i].name[0] != 0x00)
			{
				pla_file_size += file_list[i].f_size;
				if (memcmp(file_list[i].name, LAST_FILE_NAME, sizeof(file_list[i].name)) == 0x00)
				{
					return pla_file_size;
				}
				continue;
			}
		}
		break;
	}
	return 0;
}

inline BOOL PlaFileStructureIsValid(FileEx *file, FILE_ENTRY *file_list)
{
	FILE_ENTRY *file_entry = file_list;
	while (memcmp(file_entry->name, LAST_FILE_NAME, sizeof(file_entry->name)) != 0x00)
	{
		if (file->SetPointer(file_entry->offset))
		{
			BEGIN_FILE_MARKER bf_marker;
			if (sizeof(BEGIN_FILE_MARKER) == file->Read(&bf_marker, sizeof(BEGIN_FILE_MARKER)))
			{
				if (memcmp(bf_marker.magic, BEGIN_FILE_MARKER_MAGIC, 4) == 0x00)
				{
					if (memcmp(bf_marker.file_name, file_entry->name, 4) == 0x00)
					{
						if (file->SetPointer(file_entry->offset + file_entry->f_size - sizeof(END_FILE_MARKER)))
						{
							END_FILE_MARKER ef_marker;
							if (sizeof(END_FILE_MARKER) == file->Read(&ef_marker, sizeof(END_FILE_MARKER)))
							{
								if (memcmp(ef_marker.magic, END_FILE_MARKER_MAGIC, 4) == 0x00)
								{
									file_entry++;
									continue;
								}
							}
						}
					}
				}
			}
		}
		return FALSE;
	}
	return TRUE;
}

inline void MarkAsBadFile(FileEx *file)
{
	size_t buff_size = _tcsclen(file->GetName())*sizeof(TCHAR) + 2*sizeof(TCHAR) + _tcsclen(BAD_FILE_EXTENTION)*sizeof(TCHAR);
	TCHAR *buff = new TCHAR[buff_size];
	memset(buff, 0x00, buff_size);
	_stprintf_s(buff, buff_size, _T("%s.%s"), file->GetName(), BAD_FILE_EXTENTION);
	file->Rename(buff);
}

/*
DWORD WINAPI CheckPlaFile(TCHAR *file_name)
{
	FileEx file(file_name);
	LONGLONG file_size;
	printf("%s - ", file_name);
	if (file.Open(GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ) && ((file_size = file.GetSize()) > 0))
	{
		if (file_size > PLA_HEADER_SIZE + FILE_LIST_HEADER_SIZE)
		{
			BYTE rw_buffer[PLA_HEADER_SIZE + FILE_LIST_HEADER_SIZE] = {0};
			if (sizeof(rw_buffer) == file.Read(rw_buffer, sizeof(rw_buffer)))
			{
				PLA_HEADER *pla_header = (PLA_HEADER *)rw_buffer;
				FILE_LIST_HEADER *file_list_header = (FILE_LIST_HEADER *)&rw_buffer[PLA_HEADER_SIZE];
				if (PlaHeaderIsValid(pla_header) && FileListHeaderIsValid(file_list_header))
				{
					if (file_size > (PLA_HEADER_SIZE + FILE_LIST_HEADER_SIZE + file_list_header->fl_size))
					{
						BYTE *file_list_buffer = new BYTE[file_list_header->fl_size];
						memset(file_list_buffer, 0x00, file_list_header->fl_size);
						if (file_list_header->fl_size == file.Read(file_list_buffer, file_list_header->fl_size))
						{
							LONGLONG calc_file_size = 0;
							FILE_ENTRY *file_list = (FILE_ENTRY *)file_list_buffer;
							if ((calc_file_size = FileListIsValid(file_list, file_list_header->fl_size)) <= file_size)
							{
								if (file.SetFileSize(calc_file_size))
								{
									if (PlaFileStructureIsValid(&file, file_list))
									{
										printf("OK\n");
										TCHAR pla_file_name[sizeof(pla_header->name1) + sizeof(TCHAR)] = {0};
										memcpy(pla_file_name, pla_header->name1, sizeof(pla_header->name1));
										file.Rename(pla_file_name);
										return 0;
									}
								}
							}
						}
					}
				}
			}
		}
		MarkAsBadFile(&file);
		printf("BAD\n");
	}
	return 0;
}
*/

DWORD WINAPI CheckPlaFile(TCHAR *file_name)
{
	FileEx file(file_name);
	if (file.Open())
	{
		DWORD jpg_sign;
		if (sizeof(DWORD) == file.Read(&jpg_sign, sizeof(DWORD)))
		{
			if (jpg_sign == 0xDBFFD8FF)
				MarkAsBadFile(&file);
			if (jpg_sign == 0xC4FFD8FF)
				MarkAsBadFile(&file);
		}
	}
	return 0;
}

void func(void)
{
	int i, n = 20;
	{
		for (i = 0; i < n; i--)
		{
			printf("*");
		}
	}
}