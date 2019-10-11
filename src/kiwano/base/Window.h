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

#pragma once
#include <kiwano/macros.h>
#include <kiwano/core/core.h>
#include <kiwano/math/math.h>

namespace kiwano
{
	// 鼠标指针类型
	enum class CursorType
	{
		Arrow,		/* 指针 */
		TextInput,	/* 文本 */
		Hand,		/* 手指 */
		SizeAll,
		SizeNESW,
		SizeNS,
		SizeNWSE,
		SizeWE,
	};

	// 窗口设置
	struct WindowConfig
	{
		String			title;				// 标题
		std::uint32_t	width;				// 宽度
		std::uint32_t	height;				// 高度
		std::uint32_t	icon;				// 图标资源 ID
		bool			resizable;			// 窗口大小可拉伸
		bool			fullscreen;			// 全屏模式

		WindowConfig(
			String const& title = L"Kiwano Game",
			std::uint32_t width = 640,
			std::uint32_t height = 480,
			std::uint32_t icon = 0,
			bool resizable = false,
			bool fullscreen = false
		);
	};


	// 窗口
	class KGE_API Window
		: public Singleton<Window>
	{
		KGE_DECLARE_SINGLETON(Window);

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
		void SetIcon(std::uint32_t icon_resource);

		// 重设窗口大小
		void Resize(int width, int height);

		// 设置全屏模式
		void SetFullscreen(bool fullscreen, int width, int height);

		// 设置鼠标指针类型
		void SetCursor(CursorType cursor);

	public:
		void Init(WindowConfig const& config, WNDPROC proc);

		void Prepare();

		HWND GetHandle() const;

		DWORD GetWindowStyle() const;

		void UpdateWindowRect();

		void UpdateCursor();
		
		void SetActive(bool actived);

	protected:
		Window();

		~Window();

	private:
		bool		resizable_;
		bool		is_fullscreen_;
		HWND		handle_;
		int		width_;
		int		height_;
		WCHAR*		device_name_;
		CursorType	mouse_cursor_;
	};
}
