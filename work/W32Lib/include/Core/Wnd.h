#ifndef _WND
#define _WND

#include "Global\Global.h"
#include "W32App.h"
#include "WFont.h"
#include <map>

using namespace std;

namespace W32Lib
{
	class 

	class Wnd
	{
		friend class W32App;

	private:
		HWND hwnd;
		static map<HWND, Wnd *> wnd_map;
		static map<HWND, Wnd *>::iterator it;

		static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Attach(HWND handle);
		void Deattach();

	public:
		Wnd() : hwnd(NULL) {}
		virtual ~Wnd();

		HWND Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight,
			Wnd *parent, HMENU hMenu, HINSTANCE hInstance);
		HWND CreateEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth,
			int nHeight, Wnd *parent, HMENU hMenu, HINSTANCE hInstance);

		Wnd *GetWnd(HWND handle)
		{
			it = wnd_map.find(handle);
			if (it != wnd_map.end())
				return (Wnd *)(*it).second;
			else
				return NULL;
		}

		// Показать окно
		void Wnd::Show(void) {::ShowWindow(hwnd, SW_SHOW); ::UpdateWindow(hwnd);}
		void Wnd::Update(void) {::UpdateWindow(hwnd);}

		// Скрыть окно
		void Hide() {::ShowWindow(hwnd, SW_HIDE);}
		
		// The CloseWindow function minimizes (but does not destroy) the specified window.
		BOOL CloseWindow() {return ::CloseWindow(hwnd);}
		
		HWND GetHandle() {return hwnd;}

		// Возвращает контекст устройства
		HDC GetDC() {return ::GetDC(hwnd);}

		HMENU GetMenu(void) {return ::GetMenu(hwnd);}
		
		// The DestroyWindow function destroy window. The function sends WM_DESTROY
		// and WM_NCDESTROY messages to the window to deactivate it and remove the keyboard focus
		// from it. The function also destroys the window's menu, flushes the thread message queue,
		// destroys timers, removes clipboard ownership, and breaks the clipboard viewer chain (if 
		// the window is at the top of the viewer chain).
		//
		// If the specified window is a parent or owner window, DestroyWindow automatically destroys
		// the associated child or owned windows when it destroys the parent or owner window. The 
		// function first destroys child or owned windows, and then it destroys the parent or owner window.
		BOOL DestroyWindow() {return ::DestroyWindow(hwnd);}

		// The GetClientRect function retrieves the coordinates of a window's client area. The client
		// coordinates specify the upper-left and lower-right corners of the client area. Because client
		// coordinates are relative to the upper-left corner of a window's client area, the coordinates
		// of the upper-left corner are (0,0).
		BOOL GetClientRect(RECT *rect) {return ::GetClientRect(hwnd, rect);}
		
		BOOL GetWindowRect(RECT *rect) {return ::GetWindowRect(hwnd, rect);}

		// The SetForegroundWindow function puts the thread that created the specified window into the
		// foreground and activates the window. Keyboard input is directed to the window, and various
		// visual cues are changed for the user.
		BOOL SetForegroundWindow() {return ::SetForegroundWindow(hwnd);}

		// The MoveWindow function changes the position and dimensions of the window. For a top-level
		// window, the position and dimensions are relative to the upper-left corner of the screen. For
		// a child window, they are relative to the upper-left corner of the parent window's client area.
		BOOL MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint)
		{return ::MoveWindow(hwnd, X, Y, nWidth, nHeight, bRepaint);}

		// Changes the size, position, and Z order of a child, pop-up, or top-level window. These windows
		// are ordered according to their appearance on the screen. The topmost window receives the highest
		// rank and is the first window in the Z order.
		BOOL SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
		{return ::SetWindowPos(hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);}

		// Изменить размер окна
		BOOL SetWindowSize(const SIZE *sz)
		{return SetWindowPos(NULL, 0, 0, sz->cx, sz->cy, SWP_NOMOVE|SWP_NOOWNERZORDER);}

		// Переместить окно
		BOOL SetWindowPos(int new_pos_X, int new_pos_Y)
		{return SetWindowPos(NULL, new_pos_X, new_pos_Y, 0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER);}

		// Изменяет позицию и размеры окна
		BOOL SetWindowPos(int X, int Y, int width, int height)
		{return SetWindowPos(NULL, X, Y, width, height, SWP_NOOWNERZORDER);}

		BOOL CenterWindow(void);

		// Установить текст окна
		BOOL SetWindowText(LPCTSTR text) {return ::SetWindowText(hwnd, text);}

		// Установить шрифт
		//void SetFont(HFONT font) {::SendMessage(hwnd, WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);}
		void SetFont(WFont *font) {::SendMessage(hwnd, WM_SETFONT, (WPARAM)(font->GetHandle()), (LPARAM)TRUE);}

		// Возвращает текущий шрифт
		HFONT GetFont(void) {return (HFONT)::SendMessage(hwnd, WM_GETFONT, 0, 0);}

		// ICON_BIG
		//  Set the large icon for the window.
		// ICON_SMALL
		//	Set the small icon for the window.
		HICON SetIcon(HICON hicon, DWORD icon_type)
		{return (HICON)::SendMessage(hwnd, WM_SETICON, (WPARAM)icon_type, (LPARAM)hicon);}

		BOOL EnableWindow(BOOL val) {return ::EnableWindow(hwnd, val);}
		BOOL EnableWindow(void) {return ::EnableWindow(hwnd, TRUE);}
		BOOL DisableWindow(void) {return ::EnableWindow(hwnd, FALSE);}

		virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCreate(LPCREATESTRUCT pcs) {return 0;}
		virtual LRESULT OnEraseBackground(HDC hdc) {return 0;} 

		// If an application processes this message, it should return zero
		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) {return 0;}
		
		// If an application processes this message, it should return zero.
		virtual LRESULT OnSize(UINT nType, int width, int height) {return 0;}
		
		// An application should return TRUE if it processes this message.
		virtual BOOL OnSizing(WPARAM wParam, LPARAM lParam) {return TRUE;}
		
		//
		virtual void OnPaint(HDC hdc) {}
		
		//
		virtual void OnClose() {}
		
		//
		virtual void OnDestroy() {}
	};
}

#endif

