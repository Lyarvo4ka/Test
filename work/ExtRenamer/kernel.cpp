#include "stdafx.h"
#include "kernel.h"

Signature::Signature(const Signature &sign)
{
	count = sign.count;
	buff = new BYTE[count];
	memset(buff, 0x00, count);
	memcpy(buff, sign.buff, count);
}

Signature::Signature(BYTE *sign_buff, DWORD byte_count) : buff(NULL), count(byte_count)
{
	buff = new BYTE[count];
	memset(buff, 0x00, count);
	memcpy(buff, sign_buff, count);
}

void Signature::Cleanup(void)
{
	count = 0;
	delete buff;
}

FileType::FileType(const FileType &file_type)
{
	DWORD str_len = _tcslen(file_type.ext);
	ext = new TCHAR[(str_len + 1)*sizeof(TCHAR)];
	memset(ext, 0x00, (str_len + 1)*sizeof(TCHAR));
	memcpy(ext, file_type.ext, str_len*sizeof(TCHAR));
	
	sign_list = file_type.sign_list;
}

FileType::FileType(TCHAR *file_ext, Signature &sign) : ext(NULL)
{
	DWORD str_len = _tcslen(file_ext);
	ext = new TCHAR[(str_len + 1)*sizeof(TCHAR)];
	memset(ext, 0x00, (str_len + 1)*sizeof(TCHAR));
	memcpy(ext, file_ext, str_len*sizeof(TCHAR));
	sign_list.push_back(sign);
}

FileType::~FileType()
{
	Cleanup();
}

void FileType::Cleanup(void)
{
	//sign_list.clear();
	delete[] ext;
}

FileType *FileTypeList::GetFileType(BYTE *buff, DWORD buff_size)
{
	DWORD ft_count = file_type_list.size();
	for (DWORD i = 0; i < ft_count; ++i)
	{
		FileType *file_type = &file_type_list[i];
		DWORD sign_count = file_type->GetCount();
		for (DWORD j = 0; j < sign_count; ++j)
		{
			Signature *sign = file_type->GetSignture(j);
			if (buff_size >= sign->Count())
			{
				Signature *tst_sign = new Signature(buff, sign->Count());
				if (*sign == *tst_sign)
				{
					delete tst_sign;
					return file_type;
				}
				delete tst_sign;
			}
		}
	}
	return NULL;
}

