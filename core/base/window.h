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
#include "base.h"
#include "Singleton.hpp"

namespace easy2d
{
	

	class WindowImpl
	{
		E2D_DECLARE_SINGLETON(WindowImpl);

		E2D_DISABLE_COPY(WindowImpl);

	public:
		void Initialize(
			String	title,
			int		width,
			int		height,
			LPCWSTR	icon,
			bool	debug
		);

		// 获取标题
		String GetTitle() const;

		// 设置标题
		void SetTitle(const String& title);

		// 获取窗口大小
		Size GetSize() const;

		// 获取窗口宽度
		float GetWidth() const;

		// 获取窗口高度
		float GetHeight() const;

		// 重设窗口大小
		void SetSize(int width, int height);

		// 设置窗口图标
		void SetIcon(LPCWSTR icon_resource);

		HWND GetHandle() const;

		float GetContentScaleX() const;

		float GetContentScaleY() const;

	protected:
		WindowImpl();

		~WindowImpl();

	private:
		bool initialized;
		HWND handle;
		float scale_x;
		float scale_y;
	};

	E2D_DECLARE_SINGLETON_TYPE(WindowImpl, Window);
}
