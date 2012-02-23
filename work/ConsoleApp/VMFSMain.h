#pragma once
#include "W32Lib.h"
#include <map>
#include <conio.h>

typedef pair<LONGLONG,LONGLONG> CHAIN; 			// pair<pos, sz>

#define EXTENT_COUNT								409
#define SECTOR_SIZE									512
#define BLOCK_SIZE									8388608
#define START_SECTOR								34944

int VMFSMain(TCHAR* drive_name, TCHAR* chain_file_name, TCHAR* out_dir);
