#include "smart.h"

BOOL SaveStripeToFile(FileEx *stripe[], DWORD count, FileEx *out_file)
{
	DWORD rw = 0;
	BYTE *rw_buff[4];
	for (DWORD i = 0; i < count; ++i)
		rw_buff[i] = new BYTE[32*4224];

	do
	{
		for (DWORD i = 0; i < count; ++i)
		{
			if ((rw = stripe[i]->Read(&(rw_buff[i])[0], 32*4224)) < 4224)
				return FALSE;
		}

		DWORD pages = (DWORD)(rw/4224);
		for (DWORD i = 0; i < pages; ++i)
		{
			for (DWORD k = 0; k < 8; ++k)
			{
				for (DWORD j = 0; j < count; ++j)
				{
					out_file->Write(&((rw_buff[j])[i*4224 + k*(512+12)]), 512);
				}
			}
			out_file->Write(&rw_buff[0][i*4224], 16);
		}

	} while (true);


	for (DWORD i = 0; i < count; ++i)
		delete[] rw_buff[i];

	return TRUE;
}

void _tst(void)
{
	//DWORD rw = 0;
	//char *disk_name = _T("\\\\.\\PhysicalDrive1");
	//HANDLE hDisk;
	//hDisk = ::CreateFile(disk_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	//if (hDisk != INVALID_HANDLE_VALUE)
	//{
	//	DWORD out_buff_sz = sizeof(DISK_GEOMETRY_EX) + sizeof(DISK_PARTITION_INFO) + sizeof(DISK_DETECTION_INFO);
	//	BYTE *out_buffer = new BYTE[out_buff_sz];
	//	memset(out_buffer, 0x00, out_buff_sz);
	//	if (DeviceIoControl(hDisk, 
	//						IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
	//						NULL, 0,
	//						out_buffer, out_buff_sz,
	//						&rw,
	//						NULL))
	//	{
	//		PDISK_GEOMETRY_EX disk_geom = (PDISK_GEOMETRY_EX)out_buffer;
	//		PDISK_PARTITION_INFO dpi = DiskGeometryGetPartition(disk_geom);
	//		PDISK_DETECTION_INFO ddi = DiskGeometryGetDetect(disk_geom);

	//		STORAGE_PROPERTY_QUERY spq;
	//		spq.PropertyId = StorageDeviceProperty;
	//		spq.QueryType = PropertyStandardQuery;

	//		BYTE buff2[108];
	//		memset(&buff2, 0x00, 108);
	//		if (DeviceIoControl(hDisk,
	//						IOCTL_STORAGE_QUERY_PROPERTY,
	//						&spq, sizeof(STORAGE_PROPERTY_QUERY),
	//						&buff2, 108,
	//						&rw, 
	//						NULL))
	//		{
	//			PSTORAGE_DEVICE_DESCRIPTOR sdd = (PSTORAGE_DEVICE_DESCRIPTOR)buff2;

	//			char *vendorID = (char *)((BYTE *)sdd + sdd->VendorIdOffset);
	//			char *productId = (char *)((BYTE *)sdd + sdd->ProductIdOffset);
	//			char *productRevision = (char *)((BYTE *)sdd + sdd->ProductRevisionOffset);
	//			char *serialNumberOffset = (char *) ((BYTE *)sdd + sdd->SerialNumberOffset);

	//			int x = 0;
	//		}
	//	}
	//	delete out_buffer;
	//}

	//for (DWORD i = 0; i < 255; ++i)
	//{
	//	PhysicalDrive drive(i);
	//	if (drive.Open())
	//	{
	//		PDISK_GEOMETRY_EX geometry_ex = NULL;
	//		PDISK_PARTITION_INFO partition_info = NULL;
	//		PDISK_DETECTION_INFO detection_info = NULL;
	//		drive.IoctlDiskGetDriveGeometryEx(&geometry_ex, &partition_info, &detection_info);

	//		delete[] (BYTE *)geometry_ex;
	//		int x = 0;
	//	}
	//}

	//FileEx in_file(_T("F:\\stirpe1.dat"));
	//FileEx out_file(_T("F:\\vsd_1.bin"));

	//if (in_file.Open() && out_file.Create())
	//{
	//	//DWORD rw = 0;
	//	//BYTE rw_buff[16400];
	//	//while ((rw = in_file.Read(rw_buff, 32*4224)) >= 4224)
	//	//{
	//	//	DWORD pages = (DWORD)(rw/4224);
	//	//	for (DWORD i = 0; i < pages; ++i)
	//	//	{
	//	//		out_file.Write(&rw_buff[(i*4224)+ 4192], 16);
	//	//	}
	//	//}

	//	BYTE rw_buff[16384+16];
	//	while (in_file.Read(rw_buff, (16384+16)) == (16384+16))
	//	{
	//		out_file.Write(&rw_buff[16384], 16);
	//	}
	//}

	//DWORD dump_count = 16;
	//TCHAR *dump_name[] = {_T("F:\\FlashReader_2\\Data\\25347_1\\25347_1.dat"), 
	//					  _T("F:\\FlashReader_2\\Data\\25347_2\\25347_2.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_3\\25347_3.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_4\\25347_4.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_5\\25347_5.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_6\\25347_6.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_7\\25347_7.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_8\\25347_8.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_9\\25347_9.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_10\\25347_10.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_11\\25347_11.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_12\\25347_12.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_13\\25347_13.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_14\\25347_14.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_15\\25347_15.dat"),
	//					  _T("F:\\FlashReader_2\\Data\\25347_16\\25347_16.dat")};

	//FileEx *dump[16];
	//for (DWORD i = 0; i < dump_count; ++i)
	//{
	//	FileEx *file = new FileEx(dump_name[i]);
	//	if (file->Open())
	//		dump[i] = file;
	//	else
	//		return;
	//}
	//
	//FileEx *stirpe1[] = {dump[7], dump[8], dump[1], dump[2]};
	//FileEx *stirpe2[] = {dump[6], dump[9], dump[0], dump[3]};
	//FileEx *stirpe3[] = {dump[5], dump[11], dump[14], dump[4]};
	//FileEx *stirpe4[] = {dump[12], dump[10], dump[15], dump[13]};
	//
	//DWORD out_dump_count = 4;
	//TCHAR *out_dump_name[] = {_T("F:\\stirpe1.dat"), _T("F:\\stirpe2.dat"), _T("F:\\stirpe3.dat"), _T("F:\\stirpe4.dat")};
	//FileEx *out_dump[4];

	//for (DWORD i = 0; i < out_dump_count; ++i)
	//{
	//	FileEx *file = new FileEx(out_dump_name[i]);
	//	if (file->Create())
	//		out_dump[i] = file;
	//	else
	//		return;	
	//}
	//
	//
	//SaveStripeToFile(stirpe1, 4, out_dump[0]);
	//SaveStripeToFile(stirpe2, 4, out_dump[1]);
	//SaveStripeToFile(stirpe3, 4, out_dump[2]);
	//SaveStripeToFile(stirpe4, 4, out_dump[3]);


	//for (DWORD i = 0; i < dump_count; ++i)
	//	delete dump[i];

	//for (DWORD i = 0; i < out_dump_count; ++i)
	//	delete out_dump[i];

	return;
}