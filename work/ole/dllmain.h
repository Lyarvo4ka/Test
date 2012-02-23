#pragma once

#include <new>
#include "OleFile.h"

extern "C" {
	_declspec(dllexport) RawFileInterface * _stdcall GetRawFileInterface(void);
};

