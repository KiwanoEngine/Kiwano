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
#include "../base/Window.h"
#include "../renderer/Renderer.h"

namespace kiwano
{
	struct Config
	{
		WindowConfig window;	// 窗口设置
		RenderConfig render;	// 渲染设置
		bool debug;				// 调试模式

		Config(
			String const& title	= L"Kiwano Game",
			UInt32 width		= 640,
			UInt32 height		= 480,
			UInt32 icon			= 0
		);

		Config(
			WindowConfig const& wnd_config,
			RenderConfig const& render_config = RenderConfig()
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
			Config const& config = Config()
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
