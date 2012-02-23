#include "stdafx.h"

// структура pla файла
// 
// PLA_HEADER
// FILE_LIST_HEADER
// FILE_ENTRY []
// FILES
// ...

#define PLA_SIGNATURE					"WW"

typedef struct _PLA_HEADER {
	char signature[2];
	BYTE unknown1[14];
	BYTE name1[36];
	BYTE name2[36];
	BYTE unknown2[168];
} PLA_HEADER;

#define FILE_LIST_HEADER_NAME			"lTlM"

typedef struct _FILE_LIST_HEADER {
	char name[4];
	BYTE unknown[8];
	DWORD fl_size;
} FILE_LIST_HEADER;

#define LAST_FILE_NAME					"FPOE"

typedef struct _FILE_ENTRY {
	DWORD unknown;						// 0x0000
	char name[4];
	DWORD offset;
	DWORD f_size;
} FILE_ENTRY;

#define BEGIN_FILE_MARKER_MAGIC			"gBlM"
#define END_FILE_MARKER_MAGIC			"dElM"

typedef struct _BEGIN_FILE_MARKER {
	char magic[4];
	char file_name[4];
	BYTE zero[8];
} BEGIN_FILE_MARKER;

typedef struct _END_FILE_MARKER {
	char magic[4];
	BYTE zero[12];
} END_FILE_MARKER;

#define BAD_FILE_EXTENTION				_T("bad_file")
#define PLA_HEADER_SIZE					(DWORD)(sizeof(PLA_HEADER))
#define FILE_LIST_HEADER_SIZE			(DWORD)(sizeof(FILE_LIST_HEADER))

inline BOOL PlaHeaderIsValid(PLA_HEADER *header);
inline BOOL FileListHeaderIsValid(FILE_LIST_HEADER *file_list_header);
inline LONGLONG FileListIsValid(FILE_ENTRY *file_list, DWORD size);
inline BOOL PlaFileStructureIsValid(FileEx *file, FILE_ENTRY *file_list);
inline void MarkAsBadFile(FileEx *file);
DWORD WINAPI CheckPlaFile(TCHAR *file_name);
