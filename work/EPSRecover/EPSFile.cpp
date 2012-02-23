#include "StdAfx.h"
#include "EPSFile.h"

EPSFile::EPSFile(TCHAR *name, PEPSHEADER header) : FileEx(name), EPSHEADER(header)
{
}

EPSFile::~EPSFile(void)
{
}

BOOL EPSFile::IsHeader(PEPSHEADER header)
{
	return ( (header->magic == EPS_MAGIC) && (header->ps_offs == 0x20) );
}

LONGLONG EPSFile::GetSize(void)
{
	LONGLONG file_size = 0;
	file_size += ps_offs;
	file_size += ps_size;
	file_size += metafile_size;
	file_size += tiff_size;
	return file_size;
}

String EPSFile::GetTitle()
{
	String title;
	return title;
}

String EPSFile::GetCreationDate()
{
	String creation_date;
	return creation_date;
}
