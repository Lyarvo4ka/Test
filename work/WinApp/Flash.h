#ifndef _FLASH
#define _FLASH

#include "common.h"

BEGIN_NAMESPACE_FR

class Dump;

class Flash
{
private:
	vector<Dump *> dump_list;
	void ClearDumpList();

public:
	Flash(void);
	~Flash(void);
	void AddDump(Dump *dump) {dump_list.push_back(dump);}
	void AddDump(TCHAR *file_name);
	void DeleteDump(DWORD num);
	DWORD DumpCount(void) {return dump_list.size();}
	Dump *dump(DWORD num) {return dump_list[num];}
};

END_NAMESPACE_FR

#endif // _FLASH
