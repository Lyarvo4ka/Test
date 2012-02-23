#ifndef _SSTORAGE
#define _SSTORAGE

#pragma comment(lib, "ole32.lib")

class SStorage
{
private:
	IStorage *root_stg;

	HRESULT CreateStorageEx(const WCHAR *pwcsName, DWORD grfMode, STGFMT stgfmt, DWORD grfAttrs,
		STGOPTIONS *pStgOptions);
	HRESULT OpenStorageEx(const WCHAR *pwcsName, DWORD grfMode, STGFMT stgfmt, DWORD grfAttrs,
		STGOPTIONS *pStgOptions);
public:
	SStorage(void);
	~SStorage(void);

	HRESULT Create(const WCHAR *pwcsName);
	HRESULT Open(const WCHAR *pwcsName);

};

#endif // _SSTORAGE
