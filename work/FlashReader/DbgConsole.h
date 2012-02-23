#ifndef __DBGCONSOLE_H__
#define __DBGCONSOLE_H__

#ifdef SDEBUG

#include "Windows.h"

#define DEBUG_FILE		L"SDebugLog.txt"
#define DEBUG_CONSOLE	0
#define DEBUG_WAIT		1000

class SDbgConsole
{
public:
	enum TextColor
	{
		tcRed,
		tcGreen,
		tcBlue,
		tcWhite,
		tcGrey,
		tcMAXCOLOR
	};
private:
	HANDLE m_hFile;
	HANDLE m_hConsole;
	HANDLE m_hMutex;
	WORD GetTextAttr(TextColor color);
	void _Print(const wchar_t* msg, TextColor color = tcGrey);
	void _InitConsole();
public:
	SDbgConsole();
	SDbgConsole(const wchar_t * FileName);
	~SDbgConsole();
	void __cdecl Print(TextColor color, const wchar_t * Format, ...);
	void __cdecl Print(const wchar_t * Format, ...);
};

extern SDbgConsole SDBG;

#define SDbgPrint(_msg_)	SDBG.Print _msg_
#define SASSERT(exp)	\
{\
	if(!(exp)){\
		SDBG.Print(SDBG.tcRed,L"SASSERT");\
		SDBG.Print(SDBG.tcWhite,L"(%S) ",#exp);\
		SDBG.Print(L"%S, %d\r\n",__FILE__,__LINE__);\
	}\
}

#define SASSERTMSGA(exp,msg)	\
{\
	if(!(exp)){\
		SDBG.Print(SDBG.tcRed,L"%S\r\n",msg);\
		SDBG.Print(SDBG.tcRed,L"SASSERT");\
		SDBG.Print(SDBG.tcWhite,L"(%S) ",#exp);\
		SDBG.Print(L"%S, %d\r\n",__FILE__,__LINE__);\
	}\
}

#define SASSERTMSG(exp,msg)	\
{\
	if(!(exp)){\
		SDBG.Print(SDBG.tcRed,L"%s\r\n",msg);\
		SDBG.Print(SDBG.tcRed,L"SASSERT");\
		SDBG.Print(SDBG.tcWhite,L"(%S) ",#exp);\
		SDBG.Print(L"%S, %d\r\n",__FILE__,__LINE__);\
	}\
}

#else

#define SDbgPrint(_msg_)	
#define SASSERT(exp)	
#define SASSERTMSG(exp,msg)	
#define SASSERTMSGA(exp,msg)	

#endif

#endif