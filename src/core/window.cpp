// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "window.h"
#include "render.h"
#include "logs.h"
#include "../math/scalar.hpp"

#define WINDOW_STYLE			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define WINDOW_FULLSCREEN_STYLE	WS_CLIPCHILDREN | WS_POPUP
#define E2D_WND_CLASS_NAME		L"Easy2DAppWnd"

namespace easy2d
{
	namespace
	{
		void GetContentScale(float* scale_x, float* scale_y);

		void AdjustWindow(UINT width, UINT height, DWORD style, float scalex, float scaley, UINT* win_width, UINT* win_height);
	}

	Window::Window()
		: handle(nullptr)
		, scale_x(1.f)
		, scale_y(1.f)
	{
	}

	Window::~Window()
	{
		E2D_LOG(L"Destroying window");
	}

	HRESULT Window::Init(String title, int width, int height, LPCWSTR icon, bool fullscreen, WNDPROC proc, bool debug)
	{
		E2D_LOG(L"Creating window");
		
		HINSTANCE hinstance	= GetModuleHandle(nullptr);
		WNDCLASSEX wcex		= { 0 };
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.lpszClassName	= E2D_WND_CLASS_NAME;
		wcex.style			= CS_HREDRAW | CS_VREDRAW /* | CS_DBLCLKS */;
		wcex.lpfnWndProc	= proc;
		wcex.hIcon			= nullptr;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= sizeof(LONG_PTR);
		wcex.hInstance		= hinstance;
		wcex.hbrBackground	= nullptr;
		wcex.lpszMenuName	= nullptr;
		wcex.hCursor		= ::LoadCursor(nullptr, IDC_ARROW);

		if (icon)
		{
			wcex.hIcon = (HICON)::LoadImageW(
				hinstance,
				icon,
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
			);
		}
		else
		{
			wcex.hIcon = ::LoadIconW(nullptr, IDI_APPLICATION);
		}

		::RegisterClassExW(&wcex);

		int left = -1;
		int top = -1;

		HMONITOR		monitor = nullptr;
		MONITORINFOEX	monitor_info_ex;

		// Get the nearest monitor to this window
		POINT anchor;
		anchor.x = left;
		anchor.y = top;
		monitor = MonitorFromPoint(anchor, MONITOR_DEFAULTTOPRIMARY);

		// Get the target monitor info      
		memset(&monitor_info_ex, 0, sizeof(MONITORINFOEX));
		monitor_info_ex.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfoW(monitor, &monitor_info_ex);

		GetContentScale(&scale_x, &scale_y);

		if (fullscreen)
		{
			top = monitor_info_ex.rcMonitor.top;
			left = monitor_info_ex.rcMonitor.left;
		}
		else
		{
			UINT screenw = monitor_info_ex.rcWork.right - monitor_info_ex.rcWork.left;
			UINT screenh = monitor_info_ex.rcWork.bottom - monitor_info_ex.rcWork.top;

			UINT win_width, win_height;
			AdjustWindow(
				width,
				height,
				fullscreen ? (WINDOW_FULLSCREEN_STYLE) : (WINDOW_STYLE),
				scale_x,
				scale_y,
				&win_width,
				&win_height
			);

			left = monitor_info_ex.rcWork.left + (screenw - win_width) / 2;
			top = monitor_info_ex.rcWork.top + (screenh - win_height) / 2;
			width = win_width;
			height = win_height;
		}

		if (width > monitor_info_ex.rcWork.right - left)
			width = monitor_info_ex.rcWork.right - left;

		if (height > monitor_info_ex.rcWork.bottom - top)
			height = monitor_info_ex.rcWork.bottom - top;

		if (fullscreen)
		{
			DEVMODE mode;
			memset(&mode, 0, sizeof(mode));
			mode.dmSize = sizeof(DEVMODE);
			mode.dmBitsPerPel = fullscreen ? 32 : (::GetDeviceCaps(GetDC(0), BITSPIXEL));
			mode.dmPelsWidth = width;
			mode.dmPelsHeight = height;
			mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (DISP_CHANGE_SUCCESSFUL != ::ChangeDisplaySettingsExW(monitor_info_ex.szDevice, &mode, NULL, CDS_FULLSCREEN, NULL))
				logs::Errorln(L"ChangeDisplaySettings failed");
		}

		handle = ::CreateWindowExW(
			fullscreen ? (WS_EX_TOPMOST) : 0,
			E2D_WND_CLASS_NAME,
			title.c_str(),
			fullscreen ? (WINDOW_FULLSCREEN_STYLE) : (WINDOW_STYLE),
			left,
			top,
			width,
			height,
			nullptr,
			nullptr,
			hinstance,
			nullptr
		);

		if (handle == nullptr)
		{
			::UnregisterClass(E2D_WND_CLASS_NAME, hinstance);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		return S_OK;
	}

	String Window::GetTitle() const
	{
		if (handle)
		{
			wchar_t title[256];
			GetWindowTextW(handle, title, 256);
			return title;
		}
		return String();
	}

	void Window::SetTitle(String const& title)
	{
		if (handle)
			::SetWindowText(handle, title.c_str());
	}

	Size Window::GetSize() const
	{
		if (handle)
		{
			RECT rect;
			GetClientRect(handle, &rect);
			return Size{
				static_cast<float>(rect.right - rect.left),
				static_cast<float>(rect.bottom - rect.top)
			};
		}
		return Size{};
	}

	float Window::GetWidth() const
	{
		return GetSize().x;
	}

	float Window::GetHeight() const
	{
		return GetSize().y;
	}

	void Window::SetIcon(LPCWSTR icon_resource)
	{
		if (handle)
		{
			HINSTANCE hinstance = GetModuleHandle(nullptr);
			HICON icon = (HICON)::LoadImage(
				hinstance,
				icon_resource,
				IMAGE_ICON,
				0,
				0,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
			);

			::SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
			::SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		}
	}

	HWND Window::GetHandle() const
	{
		return handle;
	}

	float Window::GetContentScaleX() const
	{
		return scale_x;
	}

	float Window::GetContentScaleY() const
	{
		return scale_y;
	}

	void Window::Destroy()
	{
		if (handle)
			::DestroyWindow(handle);
	}

	namespace
	{
		void GetContentScale(float* scale_x, float* scale_y)
		{
			const float DEFAULT_SCREEN_DPI = 96.f;
			const HDC dc = GetDC(NULL);
			float xdpi = static_cast<float>(GetDeviceCaps(dc, LOGPIXELSX));
			float ydpi = static_cast<float>(GetDeviceCaps(dc, LOGPIXELSY));
			ReleaseDC(NULL, dc);

			if (scale_x)
				*scale_x = xdpi / DEFAULT_SCREEN_DPI;
			if (scale_y)
				*scale_y = ydpi / DEFAULT_SCREEN_DPI;
		}

		void AdjustWindow(UINT width, UINT height, DWORD style, float scalex, float scaley, UINT* win_width, UINT* win_height)
		{
			RECT rc;
			::SetRect(&rc, 0, 0, (int)math::Ceil(width * scalex), (int)math::Ceil(height * scaley));
			::AdjustWindowRect(&rc, style, false);

			*win_width = rc.right - rc.left;
			*win_height = rc.bottom - rc.top;

			HMONITOR monitor = ::MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
			MONITORINFO monitor_info;
			::memset(&monitor_info, 0, sizeof(MONITORINFO));
			monitor_info.cbSize = sizeof(MONITORINFO);
			::GetMonitorInfoW(monitor, &monitor_info);

			long max_width = monitor_info.rcWork.right - monitor_info.rcWork.left;
			long max_height = monitor_info.rcWork.bottom - monitor_info.rcWork.top;

			if (*win_width > static_cast<UINT>(max_width))
				*win_width = max_width;
			if (*win_height > static_cast<UINT>(max_height))
				*win_height = max_height;
		}
	}
}
