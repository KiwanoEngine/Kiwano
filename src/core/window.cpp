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

#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME
#define REGISTER_CLASS	L"Easy2DApp"

namespace easy2d
{
	namespace
	{
		void GetContentScale(float* scale_x, float* scale_y);

		Rect LocateWindow(int width, int height, float scale_x, float scale_y);
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

	HRESULT Window::Init(String title, int width, int height, LPCWSTR icon, WNDPROC proc, bool debug)
	{
		E2D_LOG(L"Creating window");

		HINSTANCE hinstance	= GetModuleHandle(nullptr);
		WNDCLASSEX wcex		= { 0 };
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.lpszClassName	= REGISTER_CLASS;
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
			wcex.hIcon = (HICON)::LoadImage(
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
			wcex.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
		}

		::RegisterClassEx(&wcex);

		GetContentScale(&scale_x, &scale_y);

		Rect client_rect = LocateWindow(width, height, scale_x, scale_y);
		handle = ::CreateWindowExW(
			NULL,
			REGISTER_CLASS,
			title.c_str(),
			WINDOW_STYLE,
			static_cast<int>(client_rect.origin.x),
			static_cast<int>(client_rect.origin.y),
			static_cast<int>(client_rect.size.x),
			static_cast<int>(client_rect.size.y),
			nullptr,
			nullptr,
			hinstance,
			nullptr
		);

		if (handle == nullptr)
		{
			::UnregisterClass(REGISTER_CLASS, hinstance);
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

	void Window::SetSize(int width, int height)
	{
		if (handle)
		{
			Rect rect = LocateWindow(width, height, scale_x, scale_y);
			::MoveWindow(
				handle,
				static_cast<int>(rect.origin.x),
				static_cast<int>(rect.origin.y),
				static_cast<int>(rect.size.x),
				static_cast<int>(rect.size.y),
				TRUE
			);
		}
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

		Rect LocateWindow(int width, int height, float scale_x, float scale_y)
		{
			int max_width = ::GetSystemMetrics(SM_CXSCREEN);
			int max_height = ::GetSystemMetrics(SM_CYSCREEN);
			RECT rect =
			{
				0,
				0,
				static_cast<LONG>(math::Ceil(width * scale_x)),
				static_cast<LONG>(math::Ceil(height * scale_y))
			};

			::AdjustWindowRectEx(&rect, WINDOW_STYLE, FALSE, NULL);
			width = static_cast<int>(rect.right - rect.left);
			height = static_cast<int>(rect.bottom - rect.top);

			if (max_width < width || max_height < height)
				logs::Warningln(L"The window is larger than screen!");

			width = std::min(width, max_width);
			height = std::min(height, max_height);

			return Rect(
				static_cast<float>((max_width - width) / 2),
				static_cast<float>((max_height - height) / 2),
				static_cast<float>(width),
				static_cast<float>(height)
			);
		}
	}
}
