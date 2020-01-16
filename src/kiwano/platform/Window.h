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
#include <kiwano/math/math.h>
#include <kiwano/core/common.h>
#include <kiwano/core/event/Event.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief 鼠标指针类型
	*/
	enum class CursorType
	{
		Arrow,		///< 指针
		TextInput,	///< 文本
		Hand,		///< 手
		SizeAll,	///< 指向四个方向的箭头
		SizeWE,		///< 指向左右方向的箭头
		SizeNS,		///< 指向上下方向的箭头
		SizeNESW,	///< 指向左下到右上方向的箭头
		SizeNWSE,	///< 指向左上到右下方向的箭头
	};

	/**
	* \~chinese
	* @brief 窗口设置
	*/
	struct WindowConfig
	{
		String		title;			///< 标题
		uint32_t	width;			///< 宽度
		uint32_t	height;			///< 高度
		uint32_t	icon;			///< 图标资源 ID
		bool		resizable;		///< 窗口大小可拉伸
		bool		fullscreen;		///< 全屏模式

		/**
		* \~chinese
		* @brief 构建窗口设置
		* @param title 标题
		* @param width 宽度
		* @param height 高度
		* @param icon 图标资源ID
		* @param resizable 窗口大小可拉伸
		* @param fullscreen 全屏模式
		*/
		WindowConfig(
			String const&	title = L"Kiwano Game",
			uint32_t		width = 640,
			uint32_t		height = 480,
			uint32_t		icon = 0,
			bool			resizable = false,
			bool			fullscreen = false
		);
	};

#if defined(KGE_WIN32)
	typedef HWND WindowHandle;
#endif


	/**
	* \~chinese
	* @brief 窗口类，控制窗口标题、大小、图标等
	*/
	class KGE_API Window
		: public Singleton<Window>
	{
		friend Singleton<Window>;

	public:
		/**
		* \~chinese
		* @brief 初始化窗口
		* @param config 窗口设置
		*/
		void Init(WindowConfig const& config);

		/**
		* \~chinese
		* @brief 获取窗口标题
		* @return 窗口标题
		*/
		String GetTitle() const;

		/**
		* \~chinese
		* @brief 获取窗口大小
		* @return 窗口大小
		*/
		Size GetSize() const;

		/**
		* \~chinese
		* @brief 获取窗口宽度
		* @return 窗口宽度
		*/
		float GetWidth() const;

		/**
		* \~chinese
		* @brief 获取窗口高度
		* @return 窗口高度
		*/
		float GetHeight() const;

		/**
		* \~chinese
		* @brief 设置标题
		* @param title 标题
		*/
		void SetTitle(String const& title);

		/**
		* \~chinese
		* @brief 设置窗口图标
		* @param icon_resource 图标资源ID
		*/
		void SetIcon(uint32_t icon_resource);

		/**
		* \~chinese
		* @brief 重设窗口大小
		* @param width 窗口宽度
		* @param height 窗口高度
		*/
		void Resize(int width, int height);

		/**
		* \~chinese
		* @brief 设置全屏模式
		* @param fullscreen 是否全屏
		* @param width 窗口宽度
		* @param height 窗口高度
		*/
		void SetFullscreen(bool fullscreen, int width, int height);

		/**
		* \~chinese
		* @brief 设置鼠标指针类型
		* @param cursor 鼠标指针类型
		*/
		void SetCursor(CursorType cursor);

		/**
		* \~chinese
		* @brief 轮询窗口事件
		* @return 返回事件队列中的第一个事件并将其从队列中移除，若事件队列为空则返回空指针
		*/
		EventPtr PollEvent();

		/**
		* \~chinese
		* @brief 获取窗口句柄
		*/
		WindowHandle GetHandle() const;

		/**
		* \~chinese
		* @brief 是否需要关闭
		*/
		bool ShouldClose();

		/**
		* \~chinese
		* @brief 销毁窗口
		*/
		void Destroy();

	private:
		Window();

		~Window();

		void PushEvent(EventPtr evt);

#if defined(KGE_WIN32)
		DWORD GetWindowStyle() const;

		void UpdateCursor();

		void SetActive(bool actived);

		static LRESULT CALLBACK WndProc(HWND, UINT32, WPARAM, LPARAM);
#endif

	private:
		bool					resizable_;
		bool					is_fullscreen_;
		WindowHandle			handle_;
		uint32_t				width_;
		uint32_t				height_;
		wchar_t*				device_name_;
		CursorType				mouse_cursor_;
		std::queue<EventPtr>	event_queue_;
	};
}
