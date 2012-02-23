#ifndef _DECRYPTOR
#define _DECRYPTOR

#include <vector>
#include "W32Lib.h"
#include "kernel.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//											Trojan.Encoder											//											
//////////////////////////////////////////////////////////////////////////////////////////////////////

//
// 1. Проверить расширение файла, относится ли оно к обрабатываемому.
// 2. Проверить сигнатуру файла,
// 3. Если сигнатура не верна - произвести дешифровку файла.
//

#define SECTOR_SIZE					512
#define	CLUSTER_SIZE				8*SECTOR_SIZE
#define CRYPT_KEY_LEN				16

class ProcessedFileTypeList;

extern BYTE crypt_key[CRYPT_KEY_LEN];
extern ProcessedFileTypeList pft_list;

void InintProcessedFileTypeList(ProcessedFileTypeList &list);
void DecryptMemBlock(BYTE *buff, DWORD size);
void Decrypt(FileEx &file, FileEx &out_file);
DWORD WINAPI DecryptProcessedFile(TCHAR *full_path);

class ProcessedFileTypeList
{
private:
	vector<FileType> list;
public:
	ProcessedFileTypeList(void) {}
	~ProcessedFileTypeList(void) {}

	void AddFileType(const FileType &file_type) {list.push_back(file_type);}
	FileType *GetFileType(TCHAR *file_ext);
	FileType *GetFileType(String &file_ext);
	FileType *GetFileType(DWORD num) {return &list[num];}
	DWORD GetCount(void) {return list.size();}
};

#endif // _DECRYPTOR