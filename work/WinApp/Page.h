#ifndef _PAGE
#define _PAGE

#include "common.h"

BEGIN_NAMESPACE_FR

class Page
{
#pragma pack(push)
#pragma pack(1)
	typedef struct CHUNK
	{
		BYTE *data;
		BYTE *vs_data;
	} CHUNK, *PCHUNK;
#pragma pack(pop)

private:
	CHUNK *chunk;

public:
	Page(/*TCHAR*/void);
	~Page(void);
};

END_NAMESPACE_FR

#endif // _PAGE
