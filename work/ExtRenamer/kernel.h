#ifndef _KERNEL
#define _KERNEL

#include <vector>
#include "W32Lib.h"

using namespace std;

class Signature
{
private:
	DWORD count;
	BYTE *buff;
	void Cleanup(void);

public:
	Signature() : buff(NULL), count(0) {}
	Signature(const Signature &sign);
	Signature(BYTE *sign_buff, DWORD byte_count);
	~Signature(void) {Cleanup();}
	DWORD Count(void) {return count;}
	BOOL operator ==(Signature &sign)
	{
		if (count != sign.count)
			return FALSE;

		for (DWORD i = 0; i < count; ++i)
		{
			if (buff[i] != sign.buff[i])
				return FALSE;
		}
		return TRUE;
	}

	BOOL operator !=(Signature &sign)
	{
		if (*this == sign)
			return FALSE;
		else
			return TRUE;
	}

	void operator =(const Signature &sign)
	{
		Cleanup();
		count = sign.count;
		buff = new BYTE[count];
		memcpy(buff, sign.buff, count);
	}
};

class FileType
{
private:
	TCHAR *ext;
	vector<Signature> sign_list;
	void Cleanup(void);
public:
	FileType(const FileType &file_type);
	FileType(TCHAR *file_ext, Signature &sign);
	~FileType(void);
	void AddSignature(const Signature &sign) {sign_list.push_back(sign);}
	Signature *GetSignture(DWORD num) {return &sign_list[num];}
	DWORD GetCount(void) {return sign_list.size();}
	TCHAR *GetFileExtension(void) {return ext;}
};

class FileTypeList
{
private:
	vector<FileType> file_type_list;
public:
	FileTypeList(void) {}
	~FileTypeList(void) {}
	void AddFileType(const FileType &file_type) {file_type_list.push_back(file_type);}
	FileType *GetFileType(BYTE *buff, DWORD buff_size);
	FileType *GetFileType(DWORD num) {return &file_type_list[num];}
	DWORD GetCount(void) {return file_type_list.size();}
};

#endif //_KERNEL




