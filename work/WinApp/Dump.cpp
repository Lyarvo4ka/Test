#include "FlashRecovery.h"

BEGIN_NAMESPACE_FR

Dump::Dump(TCHAR *file_name)
{
	if (!Open(file_name, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE))
		throw Exception(_T("Dump::Dump()"));
}

Dump::~Dump(void)
{
}

END_NAMESPACE_FR
