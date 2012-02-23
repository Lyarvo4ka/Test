#include <windows.h>

#define PAGE_SIZE								8192
#define PAGE_HEADER_SIZE						96

#pragma pack(1)
typedef struct _PAGE_HEADER {
	BYTE magic;							// must be 0x01
	BYTE unknown1[15];
	DWORD next_page;
	BYTE unknown2[12];
	DWORD number;
	BYTE unknown3[24];
	DWORD crc;
	BYTE zero_fill[32];
} PAGE_HEADER, *PPAGE_HEADER;
#pragma pack()

BOOL IsZeroFill(BYTE *buffer, DWORD count)
{
	for (DWORD i = 0; i < count; i++)
	{
		if (buffer[i] != 0x00)
			return FALSE;
	}
	return TRUE;
}

BOOL IsValidHeader(PAGE_HEADER *header)
{
	if (header->magic = 0x01 && IsZeroFill(header->zero_fill, 32))
		return TRUE;
	else
		return FALSE;
}