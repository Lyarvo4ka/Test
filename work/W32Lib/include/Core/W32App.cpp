#include "W32App.h"
#include "Wnd.h"

using namespace W32Lib;

W32MessageLoop::W32MessageLoop(HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
	: hwnd(hWnd), msg_filter_min(wMsgFilterMin), msg_filter_max(wMsgFilterMax)
{
}

DWORD W32MessageLoop::Run()
{
	MSG msg;
	BOOL ret;
	memset(&msg, 0x00, sizeof(MSG));

	while (ret = ::GetMessage(&msg, hwnd, msg_filter_min, msg_filter_max))
	{
		if (ret == -1)
		{
			//
			// For example, the function fails if hWnd is an invalid window handle
			// or lpMsg is an invalid pointer.
			//
			return ::GetLastError();
		}
		else
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
	return msg.wParam;
}

W32App::W32App(void) : W32MessageLoop(NULL, 0, 0)
{
	InitInstance();
}

W32App::~W32App(void)
{
	GdiplusShutdown(gdiplus_token);
}

void W32App::InitInstance(void)
{
	HINSTANCE hinst = ::GetModuleHandle(NULL);
	WNDCLASSEX wce;
	memset(&wce, 0x00, sizeof(WNDCLASSEX));

	wce.cbSize = sizeof(wce);
	wce.style = CS_HREDRAW | CS_VREDRAW;
	wce.lpfnWndProc = Wnd::StaticWindowProc;
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hInstance = hinst;
	wce.hIcon = ::LoadIcon(NULL, IDI_INFORMATION);
	wce.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wce.hbrBackground = /*(HBRUSH)COLOR_APPWORKSPACE*/NULL;
	wce.lpszMenuName = NULL;
	wce.lpszClassName = WND_CLASS_NAME;
	wce.hIconSm = NULL;

	if (!::RegisterClassEx(&wce)) {
		DWORD err = ::GetLastError();
		if ( err != ERROR_CLASS_ALREADY_EXISTS)
			throw W32Lib::Exception(_T("W32MainWnd::W32MainWnd():\nНе удалось зарегистрировать класс окна"), err);
	}

	//
	// Инициализация GDI++
	//
	Status ret = GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL);
	if (ret != Ok) {
		throw W32Lib::Exception(_T("W32MainWnd::W32MainWnd():\nНе удалось проинициализировать GDI+"), 0);
	}

	//
	// Инициализация стандартных элементов управления (Common Controls)
	//
	INITCOMMONCONTROLSEX icc_ex;
	memset(&icc_ex, 0x00, sizeof(INITCOMMONCONTROLSEX));
	icc_ex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc_ex.dwICC = ICC_STANDARD_CLASSES | ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_TREEVIEW_CLASSES;
	if (!InitCommonControlsEx(&icc_ex)) {
		throw W32Lib::Exception(_T("W32MainWnd::W32MainWnd():\nНе удалось принициализировать Common Controls"), 0);
	}
}
