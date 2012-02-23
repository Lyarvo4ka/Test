#include "VMFSMain.h"

LONGLONG llBe2Le(LONGLONG* big_endian)
{
	LONGLONG little_endian = 0;
	BYTE *b = (BYTE *)big_endian;
	BYTE *l = (BYTE *)&little_endian;
	l[0] = b[7]; l[1] = b[6]; l[2] = b[5]; l[3] = b[4];
	l[4] = b[3]; l[5] = b[2]; l[6] = b[1]; l[7] = b[0];
	return little_endian;
}

BOOL InitAllocChains(TCHAR* chain_file_name, map<LONGLONG, LONGLONG>* chains)
{
	FileEx file(chain_file_name);
	if (file.Open())
	{
		DWORD rw = 0;
		LONGLONG buff[EXTENT_COUNT*2] = {0};
		rw = file.Read(buff, EXTENT_COUNT*16);
		if (rw == EXTENT_COUNT*16)
		{
			for (DWORD i = 0; i < EXTENT_COUNT*2; i+=2)
			{
				LONGLONG pos = llBe2Le(&buff[i]);
				LONGLONG sz = llBe2Le(&buff[i+1]);
				chains->insert(CHAIN(pos, sz));
			}
			return TRUE;
		}
	}
	return FALSE;
}

void InitFreeChains(map<LONGLONG, LONGLONG>& alloc_chains, map<LONGLONG, LONGLONG>& free_chains)
{
	map<LONGLONG, LONGLONG>::iterator it;
	map<LONGLONG, LONGLONG>::iterator it_next;
	DWORD chain_count = alloc_chains.size();
	for (it = alloc_chains.begin(); it != alloc_chains.end();)
	{
		LONGLONG pos = (*it).first;
		LONGLONG sz = (*it).second;
		it++;
		if (it != alloc_chains.end())
		{
			LONGLONG pos_next = (*it).first;
			LONGLONG sz_next = (*it).second;
			
			if ( (pos + sz) >= pos_next )
				continue;
			else
				free_chains.insert(CHAIN(pos+sz, pos_next-(pos+sz)));					
		}
	}
}

BOOL SaveChainToFile(PhysicalDrive &drive, pair<LONGLONG,LONGLONG> chain, TCHAR *file_name)
{
	FileEx file(file_name);
	if (file.Create() && drive.IsOpen()) 
	{
		LONGLONG pos = START_SECTOR*SECTOR_SIZE + chain.first*BLOCK_SIZE;
		LONGLONG sz = chain.second;

		if (drive.SetPointer(pos))
		{
			DWORD rw = 0;
			BYTE *rw_buff = new BYTE[BLOCK_SIZE];
			memset(rw_buff, 0x00, BLOCK_SIZE);

			for (DWORD i = 0; i < sz; i++)
			{
				rw = drive.Read(rw_buff, BLOCK_SIZE);
				file.Write(rw_buff, rw);
			}
			return TRUE;
		}
	}
	return FALSE;
}

void SaveFreeChainsToDir(PhysicalDrive &drive, map<LONGLONG, LONGLONG>& free_chains, TCHAR *out_dir)
{
	DWORD chain_counter = 0;
	TCHAR file_name[1024] = {0};

	map<LONGLONG, LONGLONG>::iterator it;
	for (it = free_chains.begin(); it != free_chains.end(); it++)
	{
		memset(file_name, 0x00, 1024*sizeof(TCHAR));
		_stprintf_s(file_name, 1024, _T("%s\\CHAIN%04d.bin"), out_dir, chain_counter);
		_tprintf(_T("CHAIN #%d, POS=%d, SIZE=%d\n"), chain_counter, (*it).first, (*it).second);
		if (SaveChainToFile(drive, *it, file_name))
		{
			_tprintf(_T("Save to FILE: %s OK\n\n"), file_name);
		}
		chain_counter++;
	}
}

int VMFSMain(TCHAR *drive_name, TCHAR *chain_file_name, TCHAR *out_dir)
{
	PhysicalDrive drive(drive_name);
	map<LONGLONG, LONGLONG> alloc_chains;
	map<LONGLONG, LONGLONG> free_chains;
	if (drive.Open() && InitAllocChains(chain_file_name, &alloc_chains))
	{
		InitFreeChains(alloc_chains, free_chains);
		SaveFreeChainsToDir(drive, free_chains, out_dir);
		_tprintf(_T("\nDone. Press any key ..."));
		_getch();
	}
	return 0;
}
