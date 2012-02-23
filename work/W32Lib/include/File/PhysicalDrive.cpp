#include "PhysicalDrive.h"

using namespace W32Lib;

PhysicalDrive::PhysicalDrive(DWORD drive_number) : number(drive_number)
{
	_stprintf_s(name, MAX_PATH, _T("\\\\.\\PhysicalDrive%d"), number);
}

PhysicalDrive::PhysicalDrive(const TCHAR *drive_name) : File(drive_name)
{
	size_t sz = _tcslen(drive_name);
	for (int i = sz-1; i > 0; --i) {
		if (drive_name[i] == _T('e')) {
			number = (DWORD)_tstoi(&drive_name[i+1]);
			break;
		}
	}
}

PhysicalDrive::~PhysicalDrive(void)
{
}

BOOL PhysicalDrive::Open(const TCHAR *drive_name)
{
	if (drive_name)
		_tcscpy_s(name, MAX_DRIVE_NAME, drive_name);
	
	if (!File::Open(name))
	{
		memset(name, 0x00, sizeof(name));
		return FALSE;
	}
	else
		return TRUE;
}

BOOL PhysicalDrive::Open(DWORD dwDesiredAccess, DWORD dwShareMode)
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


BOOL PhysicalDrive::GetDeviceNumber(STORAGE_DEVICE_NUMBER *sdn)
{
	if (!sdn) return FALSE;

	DWORD rw = 0;
	return DeviceIoControl(hFile,
							IOCTL_STORAGE_GET_DEVICE_NUMBER,
							NULL, 0,
							sdn, sizeof(STORAGE_DEVICE_NUMBER),
							&rw, NULL);
}

BOOL PhysicalDrive::GetControllerNumber(DISK_CONTROLLER_NUMBER *dcn)
{
	if (!dcn) return FALSE;

	DWORD rw = 0 ;

	BOOL ret = DeviceIoControl(hFile,
								IOCTL_DISK_CONTROLLER_NUMBER,
								NULL, 0,
								dcn, sizeof(DISK_CONTROLLER_NUMBER),
								&rw, NULL);
	if (ret) return ret;
	else
	{
		DWORD err = ::GetLastError();
		int x = 0;
		return FALSE;
	}
}

BOOL PhysicalDrive::GetDriveGeometryEx(DISK_GEOMETRY_EX *disk_geometry_ex)
{
	if (!disk_geometry_ex) return FALSE;

	DWORD rw;
	return DeviceIoControl(hFile,
							IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
							NULL, 0,
							disk_geometry_ex, sizeof(DISK_GEOMETRY_EX),
							&rw, NULL);
}

BOOL PhysicalDrive::GetStorageDeviceDescriptor()
{
		DWORD rw = 0;

/*	
	typedef struct _STORAGE_PROPERTY_QUERY {
		STORAGE_PROPERTY_ID  PropertyId;
		STORAGE_QUERY_TYPE  QueryType;
		UCHAR  AdditionalParameters[1];
	} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;
*/

	STORAGE_PROPERTY_QUERY storage_property_query;
	storage_property_query.PropertyId = StorageDeviceProperty;
	storage_property_query.QueryType = PropertyStandardQuery;

/*
	typedef struct _STORAGE_DEVICE_DESCRIPTOR {
		ULONG  Version;
		ULONG  Size;
		UCHAR  DeviceType;
		UCHAR  DeviceTypeModifier;
		BOOLEAN  RemovableMedia;
		BOOLEAN  CommandQueueing;
		ULONG  VendorIdOffset;
		ULONG  ProductIdOffset;
		ULONG  ProductRevisionOffset;
		ULONG  SerialNumberOffset;
		STORAGE_BUS_TYPE  BusType;
		ULONG  RawPropertiesLength;
		UCHAR  RawDeviceProperties[1];
	} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;
*/

	STORAGE_DEVICE_DESCRIPTOR storage_device_descriptor;
	BOOL ret = DeviceIoControl(hFile, 
								IOCTL_STORAGE_QUERY_PROPERTY,
								&storage_property_query, sizeof(STORAGE_PROPERTY_QUERY),
								&storage_device_descriptor, sizeof(STORAGE_DEVICE_DESCRIPTOR),
								&rw, NULL);

	DWORD error = ::GetLastError();
 	int x = 0;

	return ret;
}

BOOL PhysicalDrive::IoctlDiskGetDriveGeometryEx(OUT PDISK_GEOMETRY_EX *geometry_ex,
												OUT PDISK_PARTITION_INFO *partition_info,
												OUT PDISK_DETECTION_INFO *detection_info)
{
	if (!geometry_ex)
		return FALSE;

	DWORD rw = 0;
	DWORD sz_buff = sizeof(DISK_GEOMETRY_EX) + 
					(detection_info ? (sizeof(DISK_PARTITION_INFO) + sizeof(DISK_DETECTION_INFO)) :
					(partition_info ? sizeof(DISK_PARTITION_INFO) : 0));

	BYTE *buff = new BYTE[sz_buff];
	memset(buff, 0x00, sz_buff);

	if (DeviceIoControl(hFile,
						IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
						NULL, 0,
						buff, sz_buff,
						&rw, NULL))
	{
		*geometry_ex = (PDISK_GEOMETRY_EX)buff;
		if (partition_info)
			*partition_info = DiskGeometryGetPartition(*geometry_ex);
		if (detection_info)
			*detection_info = DiskGeometryGetDetect(*geometry_ex);
		return TRUE;
	}
	else
		return FALSE;
}