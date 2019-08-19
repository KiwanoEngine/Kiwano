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
#include "../core/core.h"
#include "../base/time.h"
#include "../base/Component.h"
#include "../base/Event.hpp"
#include "../renderer/Color.h"

namespace kiwano
{
	struct Options
	{
		String	title;				// 标题
		Int32	width;				// 宽度
		Int32	height;				// 高度
		UInt32	icon;				// 图标资源 ID
		Color	clear_color;		// 清屏颜色
		bool	vsync;				// 垂直同步
		bool	fullscreen;			// 全屏模式
		bool	debug;				// 调试模式

		Options(
			String const& title	= L"Kiwano Game",
			Int32 width			= 640,
			Int32 height		= 480,
			UInt32 icon			= 0,
			Color clear_color	= Color::Black,
			bool vsync			= true,
			bool fullscreen		= false,
			bool debug			= false
		);
	};


	// 应用
	class KGE_API Application
		: protected noncopyable
	{
	public:
		Application();

		virtual ~Application();

		// 初始化
		void Init(
			Options const& options = Options{}
		);

		// 初始化成功时
		virtual void OnReady() {}

		// 窗口关闭时
		virtual bool OnClosing() { return true; }

		// 销毁时
		virtual void OnDestroy() {}

		// 运行
		void Run();

		// 结束
		void Quit();

		// 销毁
		void Destroy();

		// 添加组件
		void Use(
			Component* component
		);

		// 卸载组件
		void Remove(
			Component* component
		);

		// 设置时间缩放因子
		void SetTimeScale(
			Float32 scale_factor
		);

		// 分发事件
		void DispatchEvent(Event& evt);

		// 在 Kiwano 主线程中执行函数
		// 当在其他线程调用 Kiwano 函数时使用
		static void PreformInMainThread(
			Function<void()> Function
		);

	protected:
		void Render();

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UInt32, WPARAM, LPARAM);

	protected:
		bool	end_;
		bool	inited_;
		Float32	time_scale_;

		Vector<Component*>	components_;
	};
}
