#ifndef _GLOBAL
#define _GLOBAL

#include <windows.h>
//#include <WinDef.h>
#include <GdiPlus.h>
#include <tchar.h>
#include <string>

#define IN
#define OUT

#define EXCEPTION_MAX_STR_LEN		255


namespace W32Lib
{
	typedef std::basic_string<TCHAR> String;

	class Exception
	{
		private:

			DWORD error_code;
			LPCTSTR message;
			
		public:

			Exception() : error_code(::GetLastError()), message(NULL) {}
			Exception(LPCTSTR str) : error_code(::GetLastError()), message(str) {}
			Exception(LPCTSTR str, DWORD error) : error_code(error), message(str) {}
			~Exception(void) {}

			LPCTSTR GetMessage(void) {return message;}
			
			void ShowMessage(void)
			{
				LPTSTR format_message;
				String out_string = _T("");

				if (message)
				{
					out_string += message;
					out_string += _T("\n");
				}

				if (error_code)
				{
					if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
									NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									(LPTSTR)&format_message, 0, NULL))
					{
						out_string += format_message;
						::LocalFree(format_message);
					}
				}
				::MessageBox(NULL, out_string.data(), _T("W32Lib exception"), MB_OK | MB_ICONWARNING);
			}
	};
};

#endif //_GLOBAL