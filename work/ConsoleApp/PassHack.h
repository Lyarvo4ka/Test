#include "conio.h"
#include "windows.h"
#include "TCHAR.h"
#include "W32Lib.h"
#include <iostream>
#include <queue>

#define MAX_LINE_LENGTH									1024

int PassHackMain();
int BruteForce(TCHAR *module_full_name);
HWND GetStoreJetMainWindow(void);
HWND GetStoreJetEdit(HWND main_wnd);
HWND GetStoreJetLoginButton(HWND main_wnd);
HWND GetStoreJetCancelButton(HWND main_wnd);
HWND GetStoreJetErrorWindow(void);
HWND GetStoreJetEliteWindow(void);
void CloseStoreJetErrorWindow(HWND error_wnd);
HWND LoadStoreJetModule(TCHAR *module_full_name);
void UnloadStoreJetModule(void);
BOOL InputPassword(const TCHAR *pwd);

void ClosePasswordWindow(void);
void CloseErrorWindow(void);
void CloseMainWindow(void);


class Password
{
private:
	TCHAR *charset;
	DWORD charset_len;
	DWORD max_index;
	DWORD pwd_len;
	BYTE *pwd_byte;
	TCHAR *pwd_str;

public:
	Password(TCHAR *char_set, DWORD len);
	~Password();

	const TCHAR *GetPasswordString(void);

	// если произошло переполнение возвращает FALSE
	BOOL Roll(void);	
};

class TextFile
{
private:
	TCHAR *file_name;
	FileEx file;
	LONGLONG next_line_ptr;

public:
	TextFile(TCHAR *name) : file_name(name), next_line_ptr(0) {}
	BOOL Open()	{return file.Open(file_name);}
	BOOL GetLine(char line[MAX_LINE_LENGTH])
	{
		DWORD i = 0;
		LONGLONG ptr = 0; 
		BYTE r_buff[MAX_LINE_LENGTH+1] = {0};
		DWORD rw = 0;

		while (rw = file.Read(r_buff, MAX_LINE_LENGTH))
		{
			for (i = 0; i < rw; i++)
			{
				if ( (r_buff[i] == 0x0A) || (r_buff[i] == 0x0D) )
				{
					memcpy(line, r_buff, i);
					line[i] = 0x00;
					
					do
					{
						i++;
					}
					while ((r_buff[i] == 0x0A) || (r_buff[i] == 0x0D));
				}
			}
			file.GetPointer(&ptr);
			memset(r_buff, 0x00, MAX_LINE_LENGTH);
		}
		return FALSE;
	}
};
