#include "engine.h"

void PhysicalDriveMgr::UpdateDrivesList(void)
{
	for (int i = 0; i <= maxPhysicalDriveNumber; ++i)
	{
		PhysicalDrive drive(i);
		if (drive.Open())
		{
			
		}
	}
}


