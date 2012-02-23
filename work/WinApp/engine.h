#pragma once

#include <vector>
#include <W32Lib.h>

using namespace std;
using namespace W32Lib;

class PhysicalDriveMgr
{
	enum {maxPhysicalDriveNumber = 255};
private:
	vector<String> drives_list;
public:
	PhysicalDriveMgr() {}
	~PhysicalDriveMgr() {}
	void UpdateDrivesList(void);
	DWORD Count() {return (DWORD)drives_list.size();}
	PhysicalDrive *GetDrive(DWORD drive_number);
	PhysicalDrive *operator[](int index);
};

