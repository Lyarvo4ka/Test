//
// ver.:	0.0.2
//
#pragma once

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "comctl32.lib")

#include "Global\Global.h"
#include "Global\Function.h"
#include "Utilities\Utilities.h"
#include "Core\Core.h"
#include "File\File.h"
#include "File\PhysicalDrive.h"

using namespace W32Lib;
