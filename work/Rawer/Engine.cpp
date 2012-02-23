#include "stdafx.h"
#include "Engine.h"

void PhysicalDriveMgr::Clear()
{
	list<PhysicalDrive *>::iterator it;
	for (it = physical_drives_list.begin(); it != physical_drives_list.end(); it++) {

		delete *it;
	}
}

void PhysicalDriveMgr::UpdateDriveList(void)
{
	this->list<PhysicalDrive *>
}









BOOL Rawer::Run()
{
	DWORD src_files_count = List<File>::GetCount();
	DWORD files_types_count = List<RawFileInterface>::GetCount();

	if (!(src_files_count && files_types_count)) return FALSE;

	BYTE r_buffer[512] = {0};
	DWORD rw;

	for (DWORD src_file_num = 0; src_file_num < src_files_count; ++src_file_num)
	{
		File *file = List<File>::GetItem(src_file_num);
		if (!file) break;

		while (rw = file->Read(r_buffer, 512))
		{
			for (DWORD i = 0; i < files_types_count; ++i)
			{
				RawFileInterface *raw_file_interface = List<RawFileInterface>::GetItem(i);
				if (!raw_file_interface) break;

				if (raw_file_interface->IsBeginOfFile(r_buffer, 512))
				{
					
				}
			}
			memset(r_buffer, 0x00, 512);
		}
	}

	return TRUE;
}