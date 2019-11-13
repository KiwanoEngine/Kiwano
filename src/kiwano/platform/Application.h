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
#include <kiwano/core/core.h>
#include <kiwano/base/time.h>
#include <kiwano/base/Component.h>
#include <kiwano/base/Event.h>
#include <kiwano/base/Window.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	struct Config
	{
		WindowConfig window;	// 窗口设置
		RenderConfig render;	// 渲染设置
		bool debug;				// 调试模式

		Config(
			String const& title		= L"Kiwano Game",
			uint32_t width		= 640,
			uint32_t height	= 480,
			uint32_t icon		= 0
		);

		Config(
			WindowConfig const& wnd_config,
			RenderConfig const& render_config = RenderConfig()
		);
	};


	// 应用
	class KGE_API Application
		: protected core::noncopyable
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
			ComponentBase* component
		);

		// 设置时间缩放因子
		void SetTimeScale(
			float scale_factor
		);

		// 分发事件
		void DispatchEvent(Event* evt);

		// 在 Kiwano 主线程中执行函数
		// 当在其他线程调用 Kiwano 函数时使用
		static void PreformInMainThread(
			Function<void()> Function
		);

	protected:
		void Render();

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UINT32, WPARAM, LPARAM);

	protected:
		bool	end_;
		bool	inited_;
		float	time_scale_;

		Vector<ComponentBase*>		comps_;
		Vector<RenderComponent*>	render_comps_;
		Vector<UpdateComponent*>	update_comps_;
		Vector<EventComponent*>		event_comps_;
	};
}
