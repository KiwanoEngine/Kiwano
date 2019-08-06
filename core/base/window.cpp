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
#include "Game.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "../math/scalar.hpp"
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME
#define REGISTER_CLASS	L"Easy2DApp"

namespace easy2d
{
	namespace
	{
		void GetContentScale(float* scale_x, float* scale_y);

		Rect LocateWindow(int width, int height, float scale_x, float scale_y);

		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	}

	WindowImpl::WindowImpl()
		: handle(nullptr)
		, scale_x(1.f)
		, scale_y(1.f)
		, initialized(false)
	{
	}

	WindowImpl::~WindowImpl()
	{
		E2D_LOG("Destroying window");

		if (handle)
			::DestroyWindow(handle);
	}

	void WindowImpl::Init(String title, int width, int height, LPCWSTR icon, bool debug)
	{
		if (initialized)
			return;

		E2D_LOG("Initing window");

		HINSTANCE hinstance	= GetModuleHandle(nullptr);
		WNDCLASSEX wcex		= { 0 };
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.lpszClassName	= REGISTER_CLASS;
		wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc	= WndProc;
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

		::RegisterClassEx(&wcex);

		GetContentScale(&scale_x, &scale_y);

		// ���㴰�ڴ�С
		Rect client_rect = LocateWindow(width, height, scale_x, scale_y);

		// ��������
		handle = ::CreateWindowEx(
			NULL,
			REGISTER_CLASS,
			title.c_str(),
			WINDOW_STYLE,
			static_cast<int>(client_rect.origin.x),
			static_cast<int>(client_rect.origin.y),
			static_cast<int>(client_rect.size.width),
			static_cast<int>(client_rect.size.height),
			nullptr,
			nullptr,
			hinstance,
			this
		);

		if (handle == nullptr)
		{
			::UnregisterClass(REGISTER_CLASS, hinstance);
			throw std::runtime_error("Create window failed!");
		}

		// �������뷨
		::ImmAssociateContext(handle, nullptr);

		initialized = true;
	}

	String WindowImpl::GetTitle() const
	{
		if (handle)
		{
			wchar_t title[256];
			GetWindowTextW(handle, title, 256);
			return title;
		}
		return String();
	}

	void WindowImpl::SetTitle(const String& title)
	{
		if (handle)
			::SetWindowText(handle, title.c_str());
	}

	Size WindowImpl::GetSize() const
	{
		if (handle)
		{
			RECT rect;
			GetClientRect(handle, &rect);
			return Size(
				static_cast<float>(rect.right - rect.left),
				static_cast<float>(rect.bottom - rect.top)
			);
		}
		return Size();
	}

	float WindowImpl::GetWidth() const
	{
		return GetSize().width;
	}

	float WindowImpl::GetHeight() const
	{
		return GetSize().height;
	}

	void WindowImpl::SetSize(int width, int height)
	{
		if (handle)
		{
			Rect rect = LocateWindow(width, height, scale_x, scale_y);
			::MoveWindow(
				handle,
				static_cast<int>(rect.origin.x),
				static_cast<int>(rect.origin.y),
				static_cast<int>(rect.size.width),
				static_cast<int>(rect.size.height),
				TRUE
			);
		}
	}

	void WindowImpl::SetIcon(LPCWSTR icon_resource)
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

	HWND WindowImpl::GetHandle() const
	{
		return handle;
	}

	float WindowImpl::GetContentScaleX() const
	{
		return scale_x;
	}

	float WindowImpl::GetContentScaleY() const
	{
		return scale_y;
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
				logs::Warningln("The window is larger than screen!");

			width = std::min(width, max_width);
			height = std::min(height, max_height);

			return Rect(
				static_cast<float>((max_width - width) / 2),
				static_cast<float>((max_height - height) / 2),
				static_cast<float>(width),
				static_cast<float>(height)
			);
		}

		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
		{
			LRESULT result = 0;
			bool was_handled = false;

			Game * game = reinterpret_cast<Game*>(
				static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA))
				);

			switch (msg)
			{

			// ���������Ϣ
			case WM_LBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			case WM_MOUSEMOVE:
			case WM_MOUSEWHEEL:
			{
				game->Dispatch(MouseEvent(msg, w_param, l_param));
			}
			result = 0;
			was_handled = true;
			break;

			// ��������Ϣ
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				game->Dispatch(KeyEvent(msg, w_param, l_param));
			}
			result = 0;
			was_handled = true;
			break;

			// �����ڴ�С�仯��Ϣ
			case WM_SIZE:
			{
				UINT width = LOWORD(l_param);
				UINT height = HIWORD(l_param);

				// ���������յ�һ�� WM_SIZE ��Ϣ�����������������Ⱦ
				// Ŀ��Ĵ�С�������ܻ����ʧ�ܣ�����������Ժ����п��ܵ�
				// ������Ϊ�����������һ�ε��� EndDraw ʱ����
				devices::Graphics::Instance()->Resize(width, height);
			}
			break;

			// ����ֱ��ʱ仯��Ϣ
			case WM_DISPLAYCHANGE:
			{
				// �ػ�ͻ���
				::InvalidateRect(hwnd, nullptr, FALSE);
			}
			result = 0;
			was_handled = true;
			break;

			// �ػ洰��
			case WM_PAINT:
			{
				game->DrawScene();
				::ValidateRect(hwnd, nullptr);
			}
			result = 0;
			was_handled = true;
			break;

			// ���ڹر���Ϣ
			case WM_CLOSE:
			{
				if (game->OnClose())
				{
					game->Quit();
				}
			}
			result = 0;
			was_handled = true;
			break;

			// ����������Ϣ
			case WM_DESTROY:
			{
				::PostQuitMessage(0);
			}
			result = 1;
			was_handled = true;
			break;

			}

			if (!was_handled)
			{
				result = ::DefWindowProc(hwnd, msg, w_param, l_param);
			}
			return result;
		}
	}
}
