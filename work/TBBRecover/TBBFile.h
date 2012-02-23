#pragma once

#include "File.h"

#define TBB_SIGNATURE				(DWORD)0x19790620
#define MESSAGE_SIGNATURE			(DWORD)0x19700921
#define MESSAGE_END					(WORD)0x0A0D
#define MAIN_HEADER_SIZE			(DWORD)0x0C08

using namespace W32Lib;

#pragma pack (push)
#pragma pack (1)

typedef struct _MESSAGE_HEADER {
	DWORD signature;				// 21 09 70 19 - MESSAGE_SIGNATURE
	DWORD header_size;
	BYTE reserved1[8];
	DWORD index;
	BYTE reserved2[16];
	DWORD message_size;
	DWORD reserved3;
	DWORD reserved4;

	_MESSAGE_HEADER() {memset(this, 0x00, sizeof(_MESSAGE_HEADER));}
	void Erase(void) {memset(this, 0x00, sizeof(_MESSAGE_HEADER));}

} MESSAGE_HEADER, *PMESSAGE_HEADER;

typedef struct _MESSAGE {
	MESSAGE_HEADER header;
	BYTE body[1];
} MESSAGE, *PMESSAGE; 

#pragma pack (pop)

class TBBFile
{
	private:
		FileEx tbb_file;
		DWORD curr_index;

	public:
		TBBFile(void) : curr_index(1) {};
		~TBBFile(void);
		BOOL Create(TCHAR *file_name);
		BOOL IsMessageHeader(BYTE *header_buffer, DWORD size);
		BOOL AddMessage(BYTE *msg_buffer, DWORD msg_size);
		BOOL AddMessage(MESSAGE *msg);
};
