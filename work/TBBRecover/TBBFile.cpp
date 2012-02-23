#include "StdAfx.h"
#include "TBBFile.h"

TBBFile::~TBBFile(void)
{
}

BOOL TBBFile::Create(TCHAR *file_name)
{
	if (tbb_file.Create(file_name)) {
		BYTE buff[MAIN_HEADER_SIZE] = {0};
		PMESSAGE_HEADER header = (PMESSAGE_HEADER)buff;
		header->signature = TBB_SIGNATURE;
		header->header_size = MAIN_HEADER_SIZE;
		if (MAIN_HEADER_SIZE == tbb_file.Write(buff, sizeof(buff)))
			return TRUE;
	}
	return FALSE;
}

BOOL TBBFile::IsMessageHeader(BYTE *header_buffer, DWORD size)
{
	if (size < sizeof(MESSAGE_HEADER)) return FALSE;

	PMESSAGE_HEADER msg_header = (PMESSAGE_HEADER)header_buffer;

	if ((msg_header->signature == MESSAGE_SIGNATURE) && 
		(msg_header->reserved3 == 0x00) && 
		(msg_header->reserved4 == 0x00))
	{
		return TRUE;
	}
	else
		return FALSE;
}

BOOL TBBFile::AddMessage(BYTE *msg_buffer, DWORD msg_size)
{
	if (!msg_buffer && !msg_size) return FALSE;
	
	PMESSAGE_HEADER header = (PMESSAGE_HEADER)msg_buffer;

	if ((header->signature == MESSAGE_SIGNATURE) && (msg_size == (header->header_size + header->message_size))) {
		((MESSAGE_HEADER *)msg_buffer)->index = curr_index;
		if (msg_size == tbb_file.Write(msg_buffer, msg_size)) {
			++curr_index;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL TBBFile::AddMessage(MESSAGE *msg)
{
	if (!msg)
		return FALSE;
	if (!msg->header.header_size || !msg->header.message_size)
		return FALSE;

	DWORD sz = msg->header.header_size + msg->header.message_size;
	msg->header.index = curr_index;
	if (sz == tbb_file.Write(msg, sz)) {
		++curr_index;
		return TRUE;
	}
	else
		return FALSE;
}