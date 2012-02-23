#include "Wnd.h"

using namespace W32Lib;

map<HWND, Wnd *> Wnd::wnd_map;
map<HWND, Wnd *>::iterator Wnd::it;

Wnd::~Wnd(void)
{
	DestroyWindow();
}

HWND Wnd::Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight,
				Wnd *parent, HMENU hMenu, HINSTANCE hInstance)
{
	hwnd = CreateWindow(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight,
						parent ? parent->GetHandle() : NULL,
						hMenu, hInstance, (LPVOID)this);
	if (!hwnd)
		throw W32Lib::Exception(_T("Wnd::Crate():\n"), ::GetLastError());
	else
		return hwnd;
}

HWND Wnd::CreateEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth,
				int nHeight, Wnd *parent, HMENU hMenu, HINSTANCE hInstance)
{
	hwnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight,
						parent ? parent->GetHandle() : NULL,
						hMenu, hInstance, (LPVOID)this);
	if (!hwnd)
		throw W32Lib::Exception(_T("Wnd::CrateEx():\n"), ::GetLastError());
	else
		return hwnd;
}

void Wnd::Attach(HWND handle)
{
	std::pair<std::map<HWND, Wnd *>::iterator, bool> ret;
	ret = wnd_map.insert(std::pair<HWND, Wnd *>(handle, this));
	if (ret.second)
		hwnd = handle;
	else
		throw Exception(_T("Wnd::StaticWindowProc()\nwnd_map уже содержит Wnd с таким hwnd"), 0);
}

void Wnd::Deattach()
{
	wnd_map.erase(hwnd);
	hwnd = NULL;
}

BOOL Wnd::CenterWindow()
{
	RECT rc;
	int scr_X = ::GetSystemMetrics(SM_CXFULLSCREEN);
	int scr_Y = ::GetSystemMetrics(SM_CYFULLSCREEN);

	if (!scr_X || !scr_Y) return FALSE;
	if (!GetWindowRect(&rc)) return FALSE;

	return SetWindowPos((scr_X/2)-(rc.right-rc.left)/2, (scr_Y/2)-(rc.bottom-rc.top)/2);
}

//
// При создании окна с помощью CreateWindow(), приходят сообщения
// в следующем порядке:
//
// 1. WM_GETMINMAXINFO			0x0024
// 2. WM_NCCREATE				0x0081
// 3. WM_NCCALCSIZE				0x0083
// 4. WM_CREATE
//
LRESULT CALLBACK Wnd::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Wnd *wnd;
	it = wnd_map.find(hwnd);
	if (it != wnd_map.end())
	{
		wnd = (*it).second;
		if (uMsg == WM_DESTROY)
		{
			LRESULT res = wnd->WndProc(uMsg, wParam, lParam);
			wnd_map.erase(it);
			return res;
		}
		else
			return wnd->WndProc(uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
		case WM_CREATE:
		{
			if (lParam)
			{
				wnd = (Wnd *)( (LPCREATESTRUCT)lParam )->lpCreateParams;
				wnd->Attach(hwnd);
				return wnd->WndProc(uMsg, wParam, lParam);
			}
			else 
				throw Exception(_T("Wnd::StaticWindowProc()\nuMsg = WM_CREATE, lParam = NULL - ахинея"), 0);
		}
		default :
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT Wnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
		case WM_CREATE:
		{
			//
			// If an application processes this message, it should return zero to continue
			// creation of the window. If the application returns –1, the window is destroyed
			// and the CreateWindowEx or CreateWindow function returns a NULL handle.
			//
			return OnCreate((LPCREATESTRUCT)lParam);
		}
		case WM_PAINT:
		{
			hdc = BeginPaint(GetHandle(), &ps);
			if (ps.fErase)
				OnEraseBackground(hdc);
			else
				OnPaint(hdc);
			EndPaint(GetHandle(), &ps);
			return 0;
		}
		case WM_ERASEBKGND:
		{
			return OnEraseBackground((HDC)wParam);
		}
		case WM_COMMAND :
		{
			return OnCommand(wParam, lParam);
		}
		case WM_SIZE :
		{
			return OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
		}
		case WM_SIZING :
		{
			return OnSizing(wParam, lParam);
		}
		case WM_CLOSE:
		{
			OnClose();
			return 0;
		}
		case WM_DESTROY:
		{
			OnDestroy();
			::PostQuitMessage(0);
			return 0; 
		}
		default:
			return ::DefWindowProc(GetHandle(), uMsg, wParam, lParam);
	}
}



