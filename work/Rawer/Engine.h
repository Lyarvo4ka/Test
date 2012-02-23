#pragma once

#include "Plugin.h"
#include <list>


class PhysicalDriveMgr
{
	enum {maxPhysicalDriveNumber = 255};
private:
	std::list<PhysicalDrive *> physical_drives_list;
	void Clear(void);
public:
	PhysicalDriveMgr() {}
	~PhysicalDriveMgr() {Clear();}
	void UpdateDriveList(void);
};

class Rawer : public List<File>, public List<RawFileInterface>
{
private:

public:
	Rawer() {}
	~Rawer() {}

	BOOL Run();
};