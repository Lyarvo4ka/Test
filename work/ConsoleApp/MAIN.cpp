#include "stdafx.h"
#include "Calc.h"
#include "PlaChecker.h"
#include "stdafx.h"
#include <conio.h>
#include "smart.h"
#include "RAIDTools.h"
#include "PassHack.h"
#include "VMFSMain.h"
#include "cut_txt_main.h"



int _tmain(int argc, _TCHAR* argv[])
{
	int s = 0;
	int y = 0; // tst

	int ret = 0;

	if (argc >= 2)
	{
		CorrectFileSizeMain(argv[1]);
		//CutTxtMain(argv[1]);
	}

	_tprintf(_T("\nDone. Press any key ..."));
	_getch();

	return ret;
}
