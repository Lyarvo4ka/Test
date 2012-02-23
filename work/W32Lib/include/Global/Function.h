#ifndef _FUNCTION
#define _FUNCTION

#include "Global\Global.h"

//LPTSTR OpenFileDlg()
//{
//	TCHAR ret_file_name[MAX_PATH] = {0};
//	TCHAR *filter = _T("All files\0*.*\0Word files\0*.doc;*.docx\0Excel files\0*.xls;*.xlsx\0\0");
//	WORD file_name_offset = 0;
//
//	OPENFILENAME of = {0};
//	of.lStructSize	= sizeof(OPENFILENAME);
//	of.hwndOwner	= NULL;
//	of.hInstance	= NULL;
//	of.lpstrFilter	= filter;
//	of.lpstrFile	= ret_file_name;
//	of.nMaxFile		= MAX_PATH;
//	of.lpstrInitialDir = _T("d:\\Work");
//	of.lpstrTitle	= _T("Testing Open File :)");
//	of.nFileOffset	= file_name_offset;
//
//	if (GetOpenFileName(&of))
//		return of.lpstrFile;
//	else
//		return NULL;
//}


//
//	Function:
//
//	Description:
//
//	Input:
//
//	Output:
//
 

namespace W32Lib
{

	typedef DWORD (WINAPI *FOREACHFILEPROC)(TCHAR *full_path);

	//
	//	Function: ForEachFileFolder().
	//
	//	Description:
	//
	//		По указанному пути обходит каждый файл/каталог и вызывает функцию на которую указывает proc.
	//
	//	Input:
	//				
	//		path	-	указатель на строку указывающую путь. Строка не должна заканчиваться символом - '\'.
	//
	//		proc	-	указатель на функцию которая вызывается для каждого файла/каталога.
	//					typedef DWORD (WINAPI *FOREACHFILEPROC)(void *param)
	//
	//		files	-	определяет вызывать ли proc для файлов.
	//					Если TRUE - вызывать, если FALSE - нет.
	//
	//		folders -	определяет вызывать ли proc для каталогов.
	//					Если TRUE - вызывать, если FALSE - нет.	
	//
	//		checkSubFolders - определяет обрабатывать ли подкаталоги.
	//					Если TRUE - обрабатывать, если FALSE - нет.
	//
	//	Output:
	//
	//		TRUE	-	нормальное выполнение ф-ции.
	//		FALSE	-	произошла ошибка (не найден указанный путь).
	//
	//BOOL ForEachFileFolder(LPCTSTR path, FOREACHFILEPROC *proc, BOOL files, BOOL folders, BOOL checkSubFolders);
	BOOL ForEachFileFolder(LPCTSTR path, FOREACHFILEPROC file_proc, FOREACHFILEPROC folder_proc, BOOL check_subfolders);


	//
	//	Function: CmpMemBlck().
	//
	//	Description:
	//
	//		Сравнивает два блока памяти.
	//
	//	Input:
	//
	//		blck1	-	указатель на первый блок.
	//
	//		blck2	-	указатель на второй блок.
	//
	//		size	-	кол-во проверяемых байт.
	//		
	//	Output:
	//
	//		TRUE	-	блоки памяти одинаковы.
	//		FALSE	-	блоки памяти разные.
	//
	BOOL CmpMemBlck(BYTE *blck1, BYTE *blck2, DWORD size);

	String GetFileNameFromFullPathName(const String &full_path_name);
	String GetFileExtensionFromFileName(const String &file_name);
}

#endif