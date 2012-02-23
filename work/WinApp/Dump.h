#ifndef _DUMP
#define _DUMP

#include "common.h"

BEGIN_NAMESPACE_FR

class Dump : public W32Lib::FileEx
{
public:
	Dump(TCHAR *file_name);
	~Dump(void);

	//Unit *unit(LONGLONG num);
	//Block *block(LONGLONG num);
	//Page *page(LONGLONG num);

};

END_NAMESPACE_FR

#endif // _DUMP


