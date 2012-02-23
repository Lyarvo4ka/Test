#include "RAIDTools.h"
#include <iostream>

#define SECTOR_SIZE				512

typedef PhysicalDrive			*PPHYSICALDRIVE;

DWORD raid_members_count = 0;
PPHYSICALDRIVE *drives = NULL;
DWORD max_LBA_size;
DWORD sign_LBA;
TCHAR work_dir;

BOOL IsZeroFill(BYTE *buffer, DWORD count)
{
	for (DWORD i = 0; i < count; i++)
	{
		if (buffer[i] != 0x00)
			return FALSE;
	}
	return TRUE;
}

BOOL Initialize(void)
{
	while (!raid_members_count)
	{
		std::cout << "RAID members count: ";
		std::cin >> raid_members_count;
		std::cout << endl;
	}

	DWORD *drive_numbers = new DWORD[raid_members_count];

	std::cout << "Enter drive number: " << endl;
	for (DWORD i = 0; i < raid_members_count; i++)
	{
		std::cout << "#" << i << ": ";
		std::cin >> drive_numbers[i];
	}
	std::cout << endl;

	std::cout << "Opening drives: " << endl;
	drives = new PPHYSICALDRIVE[raid_members_count];
	for (DWORD i = 0; i < raid_members_count; i++)
	{
		drives[i] = new PhysicalDrive(drive_numbers[i]);
		std::cout << drives[i]->GetName();
		if (drives[i]->Open())
			std::cout << " OK\n";
		else
		{
			std::cout << " ERROR\n";
			return FALSE;
		}
	}

	delete [] drive_numbers;

	return TRUE;
}

#define BUFFER				BYTE* 

BOOL StartCheck(void)
{
	LONGLONG lba = 0;
	BYTE tmp[SECTOR_SIZE] = {0};
	BUFFER *buffers = new BUFFER[raid_members_count];
	for (DWORD i = 0; i < raid_members_count; i++)
	{
		buffers[i] = new BYTE[SECTOR_SIZE];

	}

	std::cout << endl << "Start process:\n";
	while(TRUE)
	{
		for (DWORD i = 0; i < raid_members_count; i++)
		{
			if (SECTOR_SIZE != drives[i]->Read(buffers[i], SECTOR_SIZE))
				return FALSE;
		}
		
		for (DWORD i = 0; i < raid_members_count; i++)
		{
			for (DWORD j = 0; j < SECTOR_SIZE; j++)
			{
				tmp[j] ^= (buffers[i])[j];
				int x = 0;
			}
		}

		std::cout << "LBA " << lba << ": ";

		if (IsZeroFill(tmp, SECTOR_SIZE))
		{
			cout << "XOR OK\n";
		}
		else
		{
			cout << "XOR ERROR\n";
			_tprintf(_T("                     For resume press any key ...\n"));
			_getch();
		}
		
		lba++;
		memset(tmp, 0x00, SECTOR_SIZE);
	}
}

int RAIDToolsMain(int argc, _TCHAR* argv[])
{
	if (Initialize())
	{
		StartCheck();
	}

	_tprintf(_T("For exit press any key ..."));
	_getch();

	return 0;
}
