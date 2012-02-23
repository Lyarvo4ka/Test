#include "W32Lib.h"

class PDrivesMgr
{
	enum {kMaxPhysicalDriveNumber = 128};

private:
	CList<CString, CString&> pdrives;
	POSITION pos;

public:
	PDrivesMgr();
	~PDrivesMgr();
	void Update(void);
	void Clear(void);
	CString GetFirst(void);
	CString GetNext(void);
	DWORD GetCount();
};

