#define ZIP_MAGIC		"PK"

typedef struct _ZIP_HEADER {
	BYTE	magic[2];					// ZIP_MAGIC
	WORD	sub_code;					// 0x0201 - записи с данным sub_code-ом хран€тс€ в конце файла 
										// 0x0403 - записи с данным sub_code-ом хран€тс€ в начале файла,
										//			эти записи хран€т непосредственно архивированные данные файла
										// 0x0605 - последн€€ запись, по идее она одна на весь файл
	BYTE	unknown[6];
	DWORD	time;						// ¬ќ«ћќ∆Ќќ !!! врем€
	DWORD	crc;
	DWORD	packed_size;
	DWORD	real_size;
	WORD	name_length;
	WORD	data_offset;
	BYTE	name[1];
} ZIP_HEADER;