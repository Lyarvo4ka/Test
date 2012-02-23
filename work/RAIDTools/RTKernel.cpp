#include "stdafx.h"
#include "RTKernel.h"

PDrivesMgr::PDrivesMgr() : pos(0)
{

}

PDrivesMgr::~PDrivesMgr()
{

}

void PDrivesMgr::Update()
{
	pdrives.RemoveAll();
	for (DWORD i = 0; i < kMaxPhysicalDriveNumber; ++i)
	{
		PhysicalDrive drive(i);
		if (drive.Open())
		{
			pdrives.AddTail(CString(drive.GetName()));
		}
	}
}

void PDrivesMgr::Clear(void)
{
	pdrives.RemoveAll();
}

CString PDrivesMgr::GetFirst(void)
{
	pos = pdrives.GetHeadPosition();
	if (pos)
	{
		return pdrives.GetNext(pos);
	}
	else
	{
		return CString(_T(""));
	}
	
}

CString PDrivesMgr::GetNext(void)
{
	if (pos)
	{
		return pdrives.GetNext(pos);
	}
	else
	{
		return CString(_T(""));
	}
}

DWORD PDrivesMgr::GetCount(void)
{
	return (DWORD)pdrives.GetCount();
}