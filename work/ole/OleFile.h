#pragma once

class OleFile : public RawFileInterface
{
private:

public:
	OleFile() {};
	~OleFile() {};

	// RawFileInterface
	TCHAR *GetFileType(void);
	TCHAR *GetExtension(void);
	DWORD GetMinSize(void);
	DWORD GetMaxSize(void);
	BOOL IsBeginOfFile(BYTE *buffer, DWORD size);
};
