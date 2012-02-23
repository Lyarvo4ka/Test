#include "stdafx.h"
#include "Decryptor.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											Trojan.Encoder											//											
//////////////////////////////////////////////////////////////////////////////////////////////////////

BYTE crypt_key[] = {0xBB, 0x19, 0xDA, 0xA4, 0xB4, 0x39, 0x12, 0x1B, 0x19, 0x68, 0x83, 0x44, 0xAD, 0xB9, 0x17, 0x6A};
ProcessedFileTypeList pft_list;

void InintProcessedFileTypeList(ProcessedFileTypeList &list)
{
	// *.pdf
	BYTE pdf[] = {0x25, 0x50, 0x44, 0x46};
	list.AddFileType( FileType(_T("pdf"), Signature(pdf, 4)) );

	// *.djvu
	BYTE djvu[] = {0x41, 0x54, 0x26, 0x54, 0x46, 0x4F, 0x52, 0x4D};
	list.AddFileType( FileType(_T("djvu"), Signature(djvu, 8)) );

	// *.rtf
	BYTE rtf[] = {0x7B, 0x5C, 0x72, 0x74, 0x66};
	list.AddFileType( FileType(_T("rtf"), Signature(rtf, 5)) );

	// *.doc
	BYTE doc[] = {0xD0, 0xCF, 0x11, 0xE0};
	FileType doc_file_type(_T("doc"), Signature(doc, 4));
	doc_file_type.AddSignature(Signature(rtf, 5));
	list.AddFileType(doc_file_type);

	// *.xls
	BYTE xls[] = {0xD0, 0xCF, 0x11, 0xE0};
	list.AddFileType( FileType(_T("xls"), Signature(xls, 4)) );

	// *.ppt
	BYTE ppt[] = {0xD0, 0xCF, 0x11, 0xE0};
	list.AddFileType( FileType(_T("ppt"), Signature(ppt, 4)) );

	// *.docx
	BYTE docx[] = {0x50, 0x4B, 0x03, 0x04};
	list.AddFileType( FileType(_T("docx"), Signature(docx, 4)) );

	// *.xlsx
	BYTE xlsx[] = {0x50, 0x4B, 0x03, 0x04};
	list.AddFileType( FileType(_T("xlsx"), Signature(xlsx, 4)) );

	// *.pptx
	BYTE pptx[] = {0x50, 0x4B, 0x03, 0x04};
	list.AddFileType( FileType(_T("pptx"), Signature(pptx, 4)) );

	// *.jpg
	BYTE jpg[] = {0xFF, 0xD8, 0xFF};
	list.AddFileType( FileType(_T("jpg"), Signature(jpg, 3)) );

	// *.rar
	BYTE rar[] = {0x52, 0x61, 0x72, 0x21};
	list.AddFileType( FileType(_T("rar"), Signature(rar, 4)) );

	// *.zip
	BYTE zip[] = {0x50, 0x4B, 0x03, 0x04};
	list.AddFileType( FileType(_T("zip"), Signature(zip, 4)) );

}

void DecryptMemBlock(BYTE *buff, DWORD size)
{
	register DWORD i;
	for (i = 0; i < size; ++i)
	{
		buff[i] ^= crypt_key[i];
	}
}

void Decrypt(FileEx &file, FileEx &out_file)
{
	DWORD rw = 0;
	DWORD size = 2*1024*1024;
	BYTE *rw_buff = new BYTE[size];
	memset(rw_buff, 0x00, size);
	if (rw = file.Read(rw_buff, size))
	{
		DWORD loop = (DWORD)(rw/CRYPT_KEY_LEN);
		DWORD last_bytes = rw % CRYPT_KEY_LEN;
		for (DWORD i = 0; i < loop; ++i)
		{
			DecryptMemBlock(&rw_buff[i*CRYPT_KEY_LEN], CRYPT_KEY_LEN);
		}
		if (last_bytes)
		{
			DecryptMemBlock(&rw_buff[loop*CRYPT_KEY_LEN], last_bytes);
		}
		out_file.Write(rw_buff, rw);
	}
	BYTE buff[CLUSTER_SIZE] = {0};
	while (rw = file.Read(buff, CLUSTER_SIZE))
	{
		out_file.Write(buff, rw);
	}
	delete rw_buff;
}

DWORD WINAPI DecryptProcessedFile(TCHAR *full_path)
{
	String file_name = full_path;
	String out_file_name = file_name;
	out_file_name += _T(".dcrpt");
	String file_ext = W32Lib::GetFileExtensionFromFileName(file_name);
	FileType *ftype = pft_list.GetFileType(file_ext);
	if (ftype)
	{
		FileEx file(full_path);
		FileEx out_file(out_file_name.c_str());
		if (file.Open())
		{
			BYTE buff[SECTOR_SIZE] = {0};
			if (SECTOR_SIZE == file.Read(buff, SECTOR_SIZE))
			{
				BOOL isEncrypt = TRUE;
				DWORD sign_count = ftype->GetCount();
				for (DWORD i = 0; i < sign_count; ++i)
				{
					Signature sign = *ftype->GetSignture(i);
					Signature *s = new Signature(buff, sign.Count());
					if (sign == *s)
					{
						isEncrypt = FALSE;
						break;
					}
					delete s;
				}
				if (isEncrypt)
				{
					if (out_file.Create(GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, CREATE_ALWAYS))
					{
						file.SetPointer(0);
						Decrypt(file, out_file);
					}
				}
			}
		}
	}

	return 0;
}

FileType *ProcessedFileTypeList::GetFileType(TCHAR *file_ext)
{
	if (!file_ext)
		return NULL;

	DWORD count = list.size();
	for (DWORD i = 0; i < count; ++i)
	{
		if (!_tcscmp( (&list[i])->GetFileExtension(), file_ext))
		{
			return &list[i];
		}
	}
	return NULL;
}

FileType *ProcessedFileTypeList::GetFileType(String &file_ext)
{
	DWORD count = list.size();
	for (DWORD i = 0; i < count; ++i)
	{
		if (!_tcscmp((&list[i])->GetFileExtension(), file_ext.c_str()))
		{
			return &list[i];
		}
	}
	return NULL;
}