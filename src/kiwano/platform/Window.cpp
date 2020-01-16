// Copyright (c) 2016-2018 Kiwano - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <kiwano/platform/Window.h>
#include <kiwano/platform/Application.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/core/Logger.h>

#include <imm.h>  // ImmAssociateContext
#pragma comment(lib, "imm32.lib")

#define WINDOW_FIXED_STYLE		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define WINDOW_RESIZABLE_STYLE	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX
#define WINDOW_FULLSCREEN_STYLE	WS_CLIPCHILDREN | WS_POPUP
#define KGE_WND_CLASS_NAME		L"KiwanoAppWnd"

namespace kiwano
{
	namespace
	{
		MONITORINFOEX GetMoniterInfoEx(HWND hwnd);

		void AdjustWindow(uint32_t width, uint32_t height, DWORD style, uint32_t* win_width, uint32_t* win_height);

		void ChangeFullScreenResolution(int width, int height, WCHAR* device_name);

		void RestoreResolution(WCHAR* device_name);
	}

	WindowConfig::WindowConfig(String const& title, uint32_t width, uint32_t height, uint32_t icon, bool resizable, bool fullscreen)
		: title(title)
		, width(width)
		, height(height)
		, icon(icon)
		, resizable(resizable)
		, fullscreen(fullscreen)
	{
	}

	Window::Window()
		: handle_(nullptr)
		, width_(0)
		, height_(0)
		, device_name_(nullptr)
		, is_fullscreen_(false)
		, resizable_(false)
		, mouse_cursor_(CursorType::Arrow)
	{
	}

	Window::~Window()
	{
	}

	void Window::Init(WindowConfig const& config)
	{
		HINSTANCE hinst		= GetModuleHandleW(nullptr);
		WNDCLASSEX wcex		= { 0 };
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.lpszClassName	= KGE_WND_CLASS_NAME;
		wcex.style			= CS_HREDRAW | CS_VREDRAW /* | CS_DBLCLKS */;
		wcex.lpfnWndProc	= Window::WndProc;
		wcex.hIcon			= nullptr;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= sizeof(LONG_PTR);
		wcex.hInstance		= hinst;
		wcex.hbrBackground	= (HBRUSH)(::GetStockObject(BLACK_BRUSH));
		wcex.lpszMenuName	= nullptr;
		wcex.hCursor		= ::LoadCursorW(hinst, IDC_ARROW);

		if (config.icon)
		{
			wcex.hIcon = (HICON)::LoadImageW(hinst, MAKEINTRESOURCE(config.icon), IMAGE_ICON, 0, 0,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		}

		::RegisterClassExW(&wcex);

		// Get the nearest monitor to this window
		HMONITOR monitor = ::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);

		// Get the target monitor info
		MONITORINFOEX monitor_info_ex;
		memset(&monitor_info_ex, 0, sizeof(MONITORINFOEX));
		monitor_info_ex.cbSize = sizeof(MONITORINFOEX);
		::GetMonitorInfoW(monitor, &monitor_info_ex);

		// Save the device name
		int len = lstrlenW(monitor_info_ex.szDevice);
		device_name_ = new wchar_t[len + 1];
		lstrcpyW(device_name_, monitor_info_ex.szDevice);

		uint32_t width = config.width;
		uint32_t height = config.height;
		int left = -1;
		int top = -1;

		resizable_ = config.resizable;
		is_fullscreen_ = config.fullscreen;

		if (is_fullscreen_)
		{
			top = monitor_info_ex.rcMonitor.top;
			left = monitor_info_ex.rcMonitor.left;

			if (width > static_cast<uint32_t>(monitor_info_ex.rcWork.right - left))
				width = static_cast<uint32_t>(monitor_info_ex.rcWork.right - left);

			if (height > static_cast<uint32_t>(monitor_info_ex.rcWork.bottom - top))
				height = static_cast<uint32_t>(monitor_info_ex.rcWork.bottom - top);
		}
		else
		{
			uint32_t screenw = monitor_info_ex.rcWork.right - monitor_info_ex.rcWork.left;
			uint32_t screenh = monitor_info_ex.rcWork.bottom - monitor_info_ex.rcWork.top;

			uint32_t win_width, win_height;
			AdjustWindow(width, height, GetWindowStyle(), &win_width, &win_height);

			left = monitor_info_ex.rcWork.left + (screenw - win_width) / 2;
			top = monitor_info_ex.rcWork.top + (screenh - win_height) / 2;
			width = win_width;
			height = win_height;
		}

		handle_ = ::CreateWindowExW(
			is_fullscreen_ ? WS_EX_TOPMOST : 0,
			KGE_WND_CLASS_NAME,
			config.title.c_str(),
			GetWindowStyle(),
			left,
			top,
			width,
			height,
			nullptr,
			nullptr,
			hinst,
			nullptr
		);

		if (handle_ == nullptr)
		{
			::UnregisterClass(KGE_WND_CLASS_NAME, hinst);
			win32::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
			return;
		}

		width_ = width;
		height_ = height;

		// disable imm
		::ImmAssociateContext(handle_, nullptr);

		// use Application instance in message loop
		::SetWindowLongPtr(handle_, GWLP_USERDATA, LONG_PTR(this));

		::ShowWindow(handle_, SW_SHOWNORMAL);
		::UpdateWindow(handle_);

		if (is_fullscreen_)
		{
			ChangeFullScreenResolution(width_, height_, device_name_);
		}
	}

	EventPtr Window::PollEvent()
	{
		MSG msg;
		while (::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}

		EventPtr evt;
		if (!event_queue_.empty())
		{
			evt = event_queue_.front();
			event_queue_.pop();
		}
		return evt;
	}

	String Window::GetTitle() const
	{
		if (handle_)
		{
			wchar_t title[256];
			::GetWindowTextW(handle_, title, 256);
			return title;
		}
		return String();
	}

	void Window::SetTitle(String const& title)
	{
		if (handle_)
			::SetWindowTextW(handle_, title.c_str());
	}

	Size Window::GetSize() const
	{
		return Size{
			static_cast<float>(width_),
			static_cast<float>(height_)
		};
	}

	float Window::GetWidth() const
	{
		return static_cast<float>(width_);
	}

	float Window::GetHeight() const
	{
		return static_cast<float>(height_);
	}

	void Window::SetIcon(uint32_t icon_resource)
	{
		if (handle_)
		{
			HINSTANCE hinstance = GetModuleHandle(nullptr);
			HICON icon = (HICON)::LoadImageW(
				hinstance,
				MAKEINTRESOURCE(icon_resource),
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
			);

			::SendMessage(handle_, WM_SETICON, ICON_BIG, (LPARAM)icon);
			::SendMessage(handle_, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		}
	}

	void Window::Resize(int width, int height)
	{
		if (handle_ && !is_fullscreen_)
		{
			RECT rc = { 0, 0, int(width), int(height) };
			::AdjustWindowRect(&rc, GetWindowStyle(), false);

			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			::SetWindowPos(handle_, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void Window::SetFullscreen(bool fullscreen, int width, int height)
	{
		if (is_fullscreen_ != fullscreen || width != width_ || height != height_)
		{
			is_fullscreen_ = fullscreen;

			if (is_fullscreen_)
			{
				// move window to (0, 0) before display switch
				::SetWindowPos(handle_, HWND_TOPMOST, 0, 0, width_, height_, SWP_NOACTIVATE);

				ChangeFullScreenResolution(width, height, device_name_);

				MONITORINFOEX info = GetMoniterInfoEx(handle_);

				::SetWindowLongPtr(handle_, GWL_STYLE, GetWindowStyle());
				::SetWindowPos(handle_, HWND_TOPMOST, info.rcMonitor.top, info.rcMonitor.left, width, height, SWP_NOACTIVATE);

				width_ = width;
				height_ = height;
			}
			else
			{
				RestoreResolution(device_name_);

				MONITORINFOEX info = GetMoniterInfoEx(handle_);

				uint32_t screenw = info.rcWork.right - info.rcWork.left;
				uint32_t screenh = info.rcWork.bottom - info.rcWork.top;

				uint32_t win_width, win_height;
				AdjustWindow(width, height, GetWindowStyle(), &win_width, &win_height);

				int left = screenw > win_width ? ((screenw - win_width) / 2) : 0;
				int top = screenh > win_height ? ((screenh - win_height) / 2) : 0;

				::SetWindowLongPtr(handle_, GWL_STYLE, GetWindowStyle());
				::SetWindowPos(handle_, HWND_NOTOPMOST, left, top, win_width, win_height, SWP_DRAWFRAME | SWP_FRAMECHANGED);
				
				// Update window rect
				RECT rc;
				::GetClientRect(handle_, &rc);

				width_ = static_cast<uint32_t>(rc.right - rc.left);
				height_ = static_cast<uint32_t>(rc.bottom - rc.top);
			}
			
			::ShowWindow(handle_, SW_SHOWNORMAL);
		}
	}

	void Window::SetCursor(CursorType cursor)
	{
		mouse_cursor_ = cursor;
	}

	WindowHandle Window::GetHandle() const
	{
		return handle_;
	}

	bool Window::ShouldClose()
	{
		return handle_ == nullptr;
	}

	void Window::PushEvent(EventPtr evt)
	{
		event_queue_.push(evt);
	}

	void Window::Destroy()
	{
		if (is_fullscreen_)
			RestoreResolution(device_name_);

		if (device_name_)
		{
			delete[] device_name_;
			device_name_ = nullptr;
		}

		if (handle_)
		{
			::DestroyWindow(handle_);
			handle_ = nullptr;
		}
	}

#if defined(KGE_WIN32)

	DWORD Window::GetWindowStyle() const
	{
		return is_fullscreen_ ? (WINDOW_FULLSCREEN_STYLE) : (resizable_ ? (WINDOW_RESIZABLE_STYLE) : (WINDOW_FIXED_STYLE));
	}

	void Window::UpdateCursor()
	{
		LPTSTR win32_cursor = IDC_ARROW;
		switch (mouse_cursor_)
		{
		case CursorType::Arrow:		win32_cursor = IDC_ARROW; break;
		case CursorType::TextInput:	win32_cursor = IDC_IBEAM; break;
		case CursorType::SizeAll:	win32_cursor = IDC_SIZEALL; break;
		case CursorType::SizeWE:	win32_cursor = IDC_SIZEWE; break;
		case CursorType::SizeNS:	win32_cursor = IDC_SIZENS; break;
		case CursorType::SizeNESW:	win32_cursor = IDC_SIZENESW; break;
		case CursorType::SizeNWSE:	win32_cursor = IDC_SIZENWSE; break;
		case CursorType::Hand:		win32_cursor = IDC_HAND; break;
		}
		::SetCursor(::LoadCursorW(nullptr, win32_cursor));
	}

	void Window::SetActive(bool actived)
	{
		if (!handle_)
			return;

		if (is_fullscreen_)
		{
			if (actived)
			{
				ChangeFullScreenResolution(width_, height_, device_name_);

				MONITORINFOEX info = GetMoniterInfoEx(handle_);
				::SetWindowPos(handle_, HWND_TOPMOST, info.rcMonitor.top, info.rcMonitor.left, width_, height_, SWP_NOACTIVATE);
			}
			else
			{
				RestoreResolution(device_name_);

				::SetWindowPos(handle_, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				::ShowWindow(handle_, SW_MINIMIZE);
			}
		}
	}

	LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
	{
		Window* window = reinterpret_cast<Window*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		if (window == nullptr)
		{
			return ::DefWindowProcW(hwnd, msg, wparam, lparam);
		}

		switch (msg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			KeyDownEventPtr evt = new KeyDownEvent;
			evt->code = static_cast<int>(wparam);
			window->PushEvent(evt);
		}
		break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			KeyUpEventPtr evt = new KeyUpEvent;
			evt->code = static_cast<int>(wparam);
			window->PushEvent(evt);
		}
		break;

		case WM_CHAR:
		{
			KeyCharEventPtr evt = new KeyCharEvent;
			evt->value = static_cast<char>(wparam);
			window->PushEvent(evt);
		}
		break;

		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		{
			MouseDownEventPtr evt = new MouseDownEvent;
			evt->pos = Point(((float)(int)(short)LOWORD(lparam)), ((float)(int)(short)HIWORD(lparam)));

			if		(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { evt->button = MouseButton::Left; }
			else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { evt->button = MouseButton::Right; }
			else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { evt->button = MouseButton::Middle; }

			window->PushEvent(evt);
		}
		break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			MouseUpEventPtr evt = new MouseUpEvent;
			evt->pos = Point(((float)(int)(short)LOWORD(lparam)), ((float)(int)(short)HIWORD(lparam)));

			if		(msg == WM_LBUTTONUP)	{ evt->button = MouseButton::Left; }
			else if (msg == WM_RBUTTONUP)	{ evt->button = MouseButton::Right; }
			else if (msg == WM_MBUTTONUP)	{ evt->button = MouseButton::Middle; }

			window->PushEvent(evt);
		}
		break;

		case WM_MOUSEMOVE:
		{
			MouseMoveEventPtr evt = new MouseMoveEvent;
			evt->pos = Point(((float)(int)(short)LOWORD(lparam)), ((float)(int)(short)HIWORD(lparam)));
			window->PushEvent(evt);
		}
		break;

		case WM_MOUSEWHEEL:
		{
			MouseWheelEventPtr evt = new MouseWheelEvent;
			evt->pos = Point(((float)(int)(short)LOWORD(lparam)), ((float)(int)(short)HIWORD(lparam)));
			evt->wheel = GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
			window->PushEvent(evt);
		}
		break;

		case WM_SIZE:
		{
			if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
			{
				KGE_SYS_LOG(L"Window minimized");
			}
			else
			{
				// KGE_SYS_LOG(L"Window resized");

				window->width_ = ((uint32_t)(short)LOWORD(lparam));
				window->height_ = ((uint32_t)(short)HIWORD(lparam));

				WindowResizedEventPtr evt = new WindowResizedEvent;
				evt->width = window->width_;
				evt->height = window->height_;
				window->PushEvent(evt);
			}
		}
		break;

		case WM_MOVE:
		{
			int x = ((int)(short)LOWORD(lparam));
			int y = ((int)(short)HIWORD(lparam));

			WindowMovedEventPtr evt = new WindowMovedEvent;
			evt->x = x;
			evt->y = y;
			window->PushEvent(evt);
		}
		break;

		case WM_ACTIVATE:
		{
			bool active = (LOWORD(wparam) != WA_INACTIVE);

			window->SetActive(active);

			WindowFocusChangedEventPtr evt = new WindowFocusChangedEvent;
			evt->focus = active;
			window->PushEvent(evt);
		}
		break;

		case WM_SETTEXT:
		{
			KGE_SYS_LOG(L"Window title changed");

			WindowTitleChangedEventPtr evt = new WindowTitleChangedEvent;
			evt->title.assign(reinterpret_cast<const wchar_t*>(lparam));
			window->PushEvent(evt);
		}
		break;

		case WM_SETICON:
		{
			KGE_SYS_LOG(L"Window icon changed");
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			KGE_SYS_LOG(L"The display resolution has changed");

			::InvalidateRect(hwnd, nullptr, FALSE);
		}
		break;

		case WM_SETCURSOR:
		{
			window->UpdateCursor();
		}
		break;

		case WM_CLOSE:
		{
			KGE_SYS_LOG(L"Window is closing");

			WindowClosedEventPtr evt = new WindowClosedEvent;
			window->PushEvent(evt);
			window->Destroy();
		}
		break;

		case WM_DESTROY:
		{
			KGE_SYS_LOG(L"Window was destroyed");

			::PostQuitMessage(0);
			return 0;
		}
		break;
		}

		return ::DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	namespace
	{
		MONITORINFOEX GetMoniterInfoEx(HWND hwnd)
		{
			HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFOEX monitor_info;

			memset(&monitor_info, 0, sizeof(MONITORINFOEX));
			monitor_info.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfoW(monitor, &monitor_info);

			return monitor_info;
		}

		void AdjustWindow(uint32_t width, uint32_t height, DWORD style, uint32_t* win_width, uint32_t* win_height)
		{
			RECT rc;
			::SetRect(&rc, 0, 0, (int)width, (int)height);
			::AdjustWindowRect(&rc, style, false);

			*win_width = rc.right - rc.left;
			*win_height = rc.bottom - rc.top;

			MONITORINFOEX info = GetMoniterInfoEx(NULL);

			uint32_t screenw = info.rcWork.right - info.rcWork.left;
			uint32_t screenh = info.rcWork.bottom - info.rcWork.top;

			if (*win_width > screenw)
				*win_width = screenw;
			if (*win_height > screenh)
				*win_height = screenh;
		}

		void ChangeFullScreenResolution(int width, int height, WCHAR* device_name)
		{
			DEVMODE mode;

			memset(&mode, 0, sizeof(mode));
			mode.dmSize = sizeof(DEVMODE);
			mode.dmBitsPerPel = ::GetDeviceCaps(::GetDC(0), BITSPIXEL);;
			mode.dmPelsWidth = width;
			mode.dmPelsHeight = height;
			mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (::ChangeDisplaySettingsExW(device_name, &mode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
				KGE_ERROR(L"ChangeDisplaySettings failed");
		}

		void RestoreResolution(WCHAR* device_name)
		{
			::ChangeDisplaySettingsExW(device_name, NULL, NULL, 0, NULL);
		}
	}

#endif

}
