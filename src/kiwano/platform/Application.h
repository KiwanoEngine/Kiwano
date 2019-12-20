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
#include <kiwano/common/common.h>
#include <kiwano/core/time.h>
#include <kiwano/core/Component.h>
#include <kiwano/core/Event.h>
#include <kiwano/platform/Window.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief 应用程序配置
	* @details 启动 Kiwano 应用程序前的初始化选项
	*/
	struct Config
	{
		WindowConfig window;	///< 窗口配置
		RenderConfig render;	///< 渲染配置
		bool debug;				///< 启用调试模式

		/**
		* \~chinese
		* @param title 窗口标题
		* @param width 窗口宽度
		* @param height 窗口高度
		* @param icon 窗口图标的资源ID
		*/
		Config(
			String const& title	= L"Kiwano Game",
			uint32_t width		= 640,
			uint32_t height		= 480,
			uint32_t icon		= 0
		);

		/**
		* \~chinese
		* @param wnd_config 窗口配置
		* @param render_config 渲染配置
		*/
		Config(
			WindowConfig const& wnd_config,
			RenderConfig const& render_config = RenderConfig()
		);
	};


	/**
	* \~chinese
	* @brief 应用程序，控制游戏的整个生命周期，包括初始化、启动、结束以及事件分发等
	*/
	class KGE_API Application
		: protected Noncopyable
	{
	public:
		Application();

		virtual ~Application();

		/**
		* \~chinese
		* @brief 初始化应用程序
		* @details 初始化所有功能组件后执行 OnReady 函数
		* @param config 初始化配置
		* @attention 应在使用其他功能前执行初始化，否则可能引发异常
		*/
		void Init(
			Config const& config = Config()
		);

		/**
		* \~chinese
		* @brief 初始化完成处理
		* @details 重载该函数以在应用程序初始化完成后自动执行
		*/
		virtual void OnReady() {}

		/**
		* \~chinese
		* @brief 窗口关闭处理
		* @details 重载该函数以处理用户关闭应用程序窗口时的行为
		* @return 返回 true 则正常关闭窗口，否则阻止窗口关闭
		*/
		virtual bool OnClosing() { return true; }

		/**
		* \~chinese
		* @brief 应用程序销毁处理
		* @details 重载该函数以处理应用程序销毁时的行为，如完成资源回收等
		*/
		virtual void OnDestroy() {}

		/**
		* \~chinese
		* @brief 启动应用程序
		* @note 该函数是阻塞的，应用程序结束时函数返回
		*/
		void Run();

		/**
		* \~chinese
		* @brief 终止应用程序
		*/
		void Quit();

		/**
		* \~chinese
		* @brief 销毁游戏运行过程中产生的所有资源
		*/
		void Destroy();

		/**
		* \~chinese
		* @brief 添加功能组件
		* @param[in] component 功能组件
		*/
		void Use(
			ComponentBase* component
		);

		/**
		* \~chinese
		* @brief 设置时间缩放因子
		* @details 设置时间缩放因子可等比例放大或缩小时间进度
		* @param scale_factor 缩放因子
		* @warning 设置为负数可能导致动画系统紊乱
		*/
		void SetTimeScale(
			float scale_factor
		);

		/**
		* \~chinese
		* @brief 分发事件
		* @details 将事件分发给所有事件功能组件
		* @param evt 事件
		*/
		void DispatchEvent(Event& evt);

		/**
		* \~chinese
		* @brief 在主线程中执行函数
		* @details 提供在其他线程调用 Kiwano 函数的能力
		* @param func 需要执行的函数
		*/
		static void PreformInMainThread(
			Function<void()> func
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
