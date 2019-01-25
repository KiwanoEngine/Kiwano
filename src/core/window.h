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

#pragma once
#include "include-forwards.h"
#include "Singleton.hpp"

namespace easy2d
{
	class Window
		: public ISingleton<Window>
	{
		E2D_DECLARE_SINGLETON(Window);

	public:
		// 获取标题
		String GetTitle() const;

		// 获取窗口大小
		Size GetSize() const;

		// 获取窗口宽度
		float GetWidth() const;

		// 获取窗口高度
		float GetHeight() const;

		// 设置标题
		void SetTitle(String const& title);

		// 设置窗口图标
		void SetIcon(LPCWSTR icon_resource);

		// 重设窗口大小
		void Resize(int width, int height);

		// 设置全屏模式
		void SetFullscreen(bool fullscreen, int width, int height);

		HRESULT Init(
			String	title,
			int		width,
			int		height,
			LPCWSTR	icon,
			bool	fullscreen,
			WNDPROC	proc,
			bool	debug
		);

		void Prepare();

		HWND GetHandle() const;

		DWORD GetWindowStyle() const;

		void UpdateWindowRect();

		void SetActive(bool actived);

		void Destroy();

	protected:
		Window();

		~Window();

	private:
		HWND	handle_;
		bool	is_fullscreen_;
		int		width_;
		int		height_;
		WCHAR*	device_name_;
	};
}
