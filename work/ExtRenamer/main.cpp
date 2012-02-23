#include "stdafx.h"
#include "kernel.h"
#include "W32Lib.h"
#include "Decryptor.h"
#include "Koi8rToUnicode.h"
#include "SStorage.h"
#include "SonyMTSRecovery.h"

FileTypeList file_type_list;

void InitFileTypeList(FileTypeList &file_type_list)
{
	// *.doc *.xls *.ppt ...
	BYTE ole[] = {0xD0, 0xCF, 0x11, 0xE0};
	file_type_list.AddFileType( FileType(_T("doc"), Signature(ole,4)) );

	// *.avi
	BYTE avi[] = {0x52, 0x49, 0x46, 0x46};
	file_type_list.AddFileType( FileType(_T("avi"), Signature(avi,4)) );

	// *.flac
	BYTE flac[] = {0x66, 0x4C, 0x61, 0x43};
	file_type_list.AddFileType( FileType(_T("flac"), Signature(flac, 4)) );

	// *.exe
	BYTE exe[] = {0x4D, 0x5A};
	file_type_list.AddFileType( FileType(_T("exe"), Signature(exe, 2)) );

	// *.pdf
	BYTE pdf[] = {0x25, 0x50, 0x44, 0x46};
	file_type_list.AddFileType( FileType(_T("pdf"), Signature(pdf, 4)) );

	// *.jpg
	BYTE jpg[] = {0xFF, 0xD8, 0xFF};
	file_type_list.AddFileType( FileType(_T("jpg"), Signature(jpg, 3)) );

	// *.mpg
	BYTE mpg1[] = {0x00, 0x00, 0x01, 0xB3};
	BYTE mpg2[] = {0x00, 0x00, 0x01, 0xBA};
	FileType mpg_file_type(_T("mpg"), Signature(mpg1, 4));
	mpg_file_type.AddSignature(Signature(mpg2, 4));
	file_type_list.AddFileType(mpg_file_type);

	// *.mp3
	BYTE mp31[] = {0x49, 0x44, 0x33};
	BYTE mp32[] = {0xFF, 0xFB};
	FileType mp3_file_type(_T("mp3"), Signature(mp31, 3));
	mp3_file_type.AddSignature(Signature(mp32, 2));
	file_type_list.AddFileType(mp3_file_type);

	// *.bmp
	BYTE bmp[] = {0x42, 0x4D};
	file_type_list.AddFileType( FileType(_T("bmp"), Signature(bmp, 2)) );

	// *.rtf
	BYTE rtf[] = {0x7B, 0x5C, 0x72, 0x74, 0x66};
	file_type_list.AddFileType( FileType(_T("rtf"), Signature(rtf, 5)) );

	// *.rar
	BYTE rar[] = {0x52, 0x61, 0x72};
	file_type_list.AddFileType( FileType(_T("rar"), Signature(rar, 3)) );

	// *.zip
	BYTE zip[] = {0x50, 0x4B, 0x03, 0x04};
	file_type_list.AddFileType( FileType(_T("zip"), Signature(zip, 4)) );

	// *.pb Oxygen Phone Book backup
	BYTE pb[] = {0x4F, 0x50, 0x4D, 0x32, 0x46, 0x4C, 0x48, 0x35};
	file_type_list.AddFileType( FileType(_T("pb"), Signature(pb, 8)) );

	// *.ocf Oxygen calendar
	BYTE ocf[] = {0x4F, 0x78, 0x79, 0x67, 0x65, 0x6E, 0x20, 0x43, 0x61, 0x6C, 0x65, 0x6E, 0x64, 0x61, 0x72};
	file_type_list.AddFileType( FileType(_T("ocf"), Signature(ocf, 15)) );

	// *.obk Oxygen backup
	BYTE obk[] = {0x4D, 0x53, 0x43, 0x46};
	file_type_list.AddFileType( FileType(_T("obk"), Signature(obk, 4)) );

	// *.flv Oxygen backup
	BYTE flv[] = {0x46, 0x4C, 0x56};
	file_type_list.AddFileType( FileType(_T("flv"), Signature(flv, 3)) );

	// *.rrf
	BYTE rrf[] = {0x20, 0x4C, 0x56};
	file_type_list.AddFileType( FileType(_T("rrf"), Signature(rrf, 3)) );
}

DWORD WINAPI CorrectFileExtension(TCHAR *full_path)
{
	String file_full_name = full_path;
	String real_file_ext = W32Lib::GetFileExtensionFromFileName(file_full_name);
	FileEx file(full_path);
	if (file.Open())
	{
		BYTE sector[512] = {0};
		if (512 == file.Read(sector, 512))
		{
			FileType *ftype = file_type_list.GetFileType(sector, 512);
			if (ftype)
			{
				String ext_by_sign = ftype->GetFileExtension();
				if (real_file_ext.compare(ext_by_sign))
				{
					file_full_name += _T(".");
					file_full_name += ext_by_sign;
					file.Rename(file_full_name.c_str());
				}
			}
		}
	}	
	return 0;
}

void TestFunc(void);

DWORD WINAPI TestMaxFile(TCHAR *file_name)
{
	HRESULT result;
	IStorage *root_stg;
	IEnumSTATSTG *penum;
	STATSTG statstg;
	FileEx file(file_name);
	String cmp_str = _T("Scene");

	result = StgOpenStorageEx(file_name,
		STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
		STGFMT_STORAGE, 0, 0, NULL, IID_IStorage, (void **)&root_stg);
	if (result == S_OK)
	{
		result = root_stg->EnumElements(0, NULL, NULL, &penum);
		if (result == S_OK)
		{
			result = penum->Next(1, &statstg, NULL);
			while (result == S_OK)
			{
				if (statstg.type == STGTY_STREAM)
				{
					String elem_name = statstg.pwcsName;
					if (elem_name == cmp_str)
					{
						String fname = file_name;
						fname += _T(".max");
						root_stg->Release();
						penum->Release();
						if (file.Open())
							file.Rename(fname.c_str());
						return 0;
					}
					int x = 0;
				}
				result = penum->Next(1, &statstg, NULL);
			}
			CoTaskMemFree(statstg.pwcsName);
			statstg.pwcsName = NULL;
			penum->Release();
		}
		root_stg->Release();
	}
	return 0;
}

#define BAD_FILE_EXT			_T(".bad_file")
#define RRF_FILE_EXT			_T("rrf")

DWORD WINAPI CheckRrfFiles(TCHAR *file_name)
{
	BYTE rw_buff[512] = {0};
	DWORD rw;
	String f_name = file_name;
	String bad_f_name = f_name + BAD_FILE_EXT;
	String file_ext = W32Lib::GetFileExtensionFromFileName(f_name);

	if (file_ext == RRF_FILE_EXT)
	{
		FileEx file(file_name);
		if (file.Open())
		{
			if (rw = file.Read(rw_buff, 512))
			{
				if (rw_buff[0] == 0x20)
				{
					file.SetPointer(-2,FILE_END);
					WORD nl;
					if ( sizeof(nl) == file.Read(&nl, sizeof(nl)) )
					{
						if (nl == 0x0A0D)
						{
							return 0;
						}
					}
				}
				file.Rename(bad_f_name.c_str());
			}
		}
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//InitFileTypeList(file_type_list);
	//
	////InintProcessedFileTypeList(pft_list);

	//if (argc >= 2)
	//{
		//ForEachFileFolder(argv[1], CorrectFileExtension, NULL, TRUE);
		//ForEachFileFolder(argv[1], DecryptProcessedFile, NULL, TRUE);
		//ForEachFileFolder(argv[1], TestMaxFile, NULL, TRUE);
	//}
	//return 0;
																			
	////////////////////////////////////////////////////////////////////////
	// Sony MTS Video recovery

	//if (argc >= 3)
	//{
	//	String in_file_name = argv[1];
	//	String out_folder_name = argv[2];
	//	Run(in_file_name, out_folder_name);
	//}

	////////////////////////////////////////////////////////////////////////
	// Check *.rrf files

	//if (argc >= 2)
	//{
		String input_dir = argv[1];
		ForEachFileFolder(argv[1], CheckRrfFiles, NULL, TRUE);
	//}


	return 0;
}


