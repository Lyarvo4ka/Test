#include "file.h"

using namespace W32Lib;
////////////////////////////////////////////////////////////
// class File
//

BOOL File::Open()
{
	hFile = CreateFile(name,
						GENERIC_READ/*|GENERIC_WRITE*/,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

BOOL File::Open(const TCHAR *file_name)
{
	hFile = CreateFile(file_name,
						GENERIC_READ/*|GENERIC_WRITE*/,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		_tcscpy_s(name, MAX_PATH, file_name);
		return TRUE;
	}
}

BOOL File::Create()
{
	hFile = CreateFile(name,
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,
						CREATE_NEW,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

BOOL File::Create(const TCHAR *file_name)
{
	hFile = CreateFile(file_name,
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						NULL,
						CREATE_NEW,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		_tcscpy_s(name, MAX_PATH, file_name);
		return TRUE;
	}
}

void File::Close(void)
{
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
}

DWORD File::Read(void *buffer, DWORD count)
{
	DWORD rw = 0;
	if (ReadFile(hFile, buffer, count, &rw, NULL))
		return rw;
	else
		return 0;
}

DWORD File::Write(void *buffer, DWORD count)
{
	DWORD rw = 0;
	if (WriteFile(hFile, buffer, count, &rw, NULL))
		return rw;
	else
		return 0;
}

BOOL File::SetPointer(LONGLONG pointer, DWORD dwMoveMethod /*= FILE_BEGIN*/)
{
	LARGE_INTEGER distance_to_move;
	distance_to_move.QuadPart = pointer;
	if (SetFilePointerEx(hFile, distance_to_move, NULL, dwMoveMethod))
		return TRUE;
	else
		return FALSE;
}

BOOL File::GetPointer(LONGLONG *pointer)
{
	LARGE_INTEGER ret_pointer;
	LARGE_INTEGER zero_distance;
	zero_distance.QuadPart = 0;
	if (SetFilePointerEx(hFile, zero_distance, &ret_pointer, FILE_CURRENT)) {
		*pointer = ret_pointer.QuadPart;
		return TRUE;
	}
	else
		return FALSE;
}


////////////////////////////////////////////////////////////
// class FileEx
//

BOOL FileEx::Open(DWORD dwDesiredAccess, DWORD dwShareMode)
{
	hFile = CreateFile(name,
						dwDesiredAccess,
						dwShareMode,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

BOOL FileEx::Open(const TCHAR *file_name, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	hFile = CreateFile(file_name,
						dwDesiredAccess,
						dwShareMode,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		_tcscpy_s(name, MAX_PATH, file_name);
		return TRUE;
	}
}

BOOL FileEx::Create(DWORD dwDesiredAccess, DWORD dwShareMode)
{
	hFile = CreateFile(name,
						dwDesiredAccess,
						dwShareMode,
						NULL,
						CREATE_NEW,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

BOOL FileEx::Create(const TCHAR *file_name, DWORD dwDesiredAccess, DWORD dwShareMode)
{
	hFile = CreateFile(file_name,
						dwDesiredAccess,
						dwShareMode,
						NULL,
						CREATE_NEW,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		_tcscpy_s(name, MAX_PATH, file_name);
		return TRUE;
	}
}

BOOL FileEx::Create(DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition)
{
	hFile = CreateFile(name,
						dwDesiredAccess,
						dwShareMode,
						NULL,
						dwCreationDisposition,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

BOOL FileEx::Create(const TCHAR *file_name, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition)
{
	hFile = CreateFile(file_name,
						dwDesiredAccess,
						dwShareMode,
						NULL,
						dwCreationDisposition,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		_tcscpy_s(name, MAX_PATH, file_name);
		return TRUE;
	}
}

BOOL FileEx::SetPointer(LONGLONG pointer, LONGLONG *new_pointer, DWORD dwMoveMethod)
{
	LARGE_INTEGER distance_to_move;
	LARGE_INTEGER new_file_pointer;
	distance_to_move.QuadPart = pointer;
	new_file_pointer.QuadPart = 0;
	if (SetFilePointerEx(hFile, distance_to_move, &new_file_pointer, dwMoveMethod)) {
		if (new_pointer)
			*new_pointer =  new_file_pointer.QuadPart;
		return TRUE;
	}
	else return FALSE;
}

LONGLONG FileEx::GetSize(void)
{
	LARGE_INTEGER size;
	if (::GetFileSizeEx(hFile, &size))
		return size.QuadPart;
	else
		return (LONGLONG)(-1);
}

BOOL FileEx::SetFileSize(LONGLONG new_size)
{
	if(SetPointer(new_size))
	{
		if(SetEndOfFile())
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL FileEx::Rename(const TCHAR *new_name)
{
	BOOL isOpen;
	
	if (isOpen = IsOpen())
		Close();

	if (MoveFile(name, new_name)) {
		_tcscpy_s(name, MAX_PATH, new_name);
		if (isOpen)
			Open();
		return TRUE;
	}
	else
		return FALSE;
}

#define BUFFER_SIZE		(DWORD)8*512

LONGLONG FileEx::Find(BYTE *str, DWORD size)
{
	if (!str || ! size || (size > BUFFER_SIZE)) return -1;

	BYTE r_buffer[BUFFER_SIZE];
	DWORD rw;
	LONGLONG file_ptr;

	// Сохраняем текущий вайловый указатель
	GetPointer(&file_ptr);
	
	while ((rw = Read(r_buffer, BUFFER_SIZE)) && (rw >= size))
	{
		for (DWORD i = 0; (i + size) <= rw; ++i)
		{
			if (CmpMemBlck(&r_buffer[i], str, size))
			{	// Нашли искомую строку
				LONGLONG fptr;
				// Устанавливаем файловый указатель на начало строки
				SetPointer(-(LONGLONG)(rw - i), FILE_CURRENT);
				GetPointer(&fptr);
				return fptr;
			}
		}
		SetPointer(-(LONGLONG)(size-1), FILE_CURRENT);
	}

	// Восстанавливаем файловый указатель
	SetPointer(file_ptr);

	return -1;
}

BOOL FileEx::Copy(IFile *source_file)
{
	if (!source_file)
		return FALSE;

	if (!source_file->IsOpen())
		if (!source_file->Open())
			return FALSE;

	if (!this->IsOpen())
		if (!this->Open())
			return FALSE;

	if (!source_file->SetPointer(0, FILE_BEGIN))
		return FALSE;

	if (!this->File::SetPointer(0, FILE_BEGIN))
		return FALSE;

	BYTE rw_buffer[8*512];
	DWORD rw = 0;

	while (rw = source_file->Read(rw_buffer, 8*512))
		this->Write(rw_buffer, rw);

	return TRUE;
}