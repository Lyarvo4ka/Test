#include "StdAfx.h"
#include "SStorage.h"

SStorage::SStorage(void) : root_stg(NULL)
{
}

SStorage::~SStorage(void)
{
	if (!root_stg)
		root_stg->Release();
}

HRESULT SStorage::CreateStorageEx(const WCHAR *pwcsName, DWORD grfMode, STGFMT stgfmt, DWORD grfAttrs,
	STGOPTIONS *pStgOptions)
{
	return ::StgCreateStorageEx(pwcsName, grfMode, stgfmt, grfAttrs, pStgOptions,
		NULL, IID_IStorage, (void**)&root_stg);
}

HRESULT SStorage::OpenStorageEx(const WCHAR *pwcsName, DWORD grfMode, STGFMT stgfmt, DWORD grfAttrs,
	STGOPTIONS *pStgOptions)
{
	return ::StgOpenStorageEx(pwcsName, grfMode, stgfmt, grfAttrs, pStgOptions,
		0, IID_IStorage, (void**)&root_stg);
}

HRESULT SStorage::Create(const WCHAR *pwcsName)
{
	return CreateStorageEx(pwcsName, STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
		STGFMT_STORAGE, 0, NULL);
}

HRESULT SStorage::Open(const WCHAR *pwcsName)
{
	return OpenStorageEx(pwcsName, STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
		STGFMT_STORAGE, 0, NULL);
}

