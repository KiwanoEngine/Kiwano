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
#include "time.h"
#include "window.h"
#include "input.h"
#include "audio.h"

namespace easy2d
{
	struct Options
	{
		String	title;				// 标题
		int		width;				// 宽度
		int		height;				// 高度
		LPCWSTR	icon;				// 图标
		Color	clear_color;		// 清屏颜色
		bool	vsync;				// 垂直同步
		bool	fullscreen;			// 全屏模式

		Options()
			: title(L"Easy2D Game")
			, width(640)
			, height(480)
			, icon(nullptr)
			, clear_color(Color::Black)
			, vsync(true)
			, fullscreen(false)
		{}
	};


	class E2D_API Application
		: protected Noncopyable
	{
	public:
		Application(
			String const& app_name = L"Easy2dGame"
		);

		virtual ~Application();

		// 初始化
		void Init(
			Options const& options = Options{}
		);

		// 启动时
		virtual void OnStart() {}

		// 关闭时
		virtual bool OnClosing() { return true; }

		// 销毁时
		virtual void OnDestroy() {}

		// 渲染时
		virtual void OnRender() {}

		// 更新时
		virtual void OnUpdate(Duration dt) { E2D_NOT_USED(dt); }

		// 运行
		void Run();

		// 结束
		void Quit();

		// 销毁
		void Destroy();

		// 切换场景
		void EnterScene(
			ScenePtr const& scene			/* 场景 */
		);

		// 切换场景
		void EnterScene(
			ScenePtr const& scene,			/* 场景 */
			TransitionPtr const& transition	/* 场景动画 */
		);

		// 获取当前场景
		ScenePtr const& GetCurrentScene();

		// 设置时间缩放因子
		void SetTimeScale(
			float scale_factor
		);

		// 设置消息预处理函数
		void SetPreMessageProc(
			LRESULT (*proc)(HWND, UINT, WPARAM, LPARAM)
		);

		// 显示调试信息
		void ShowDebugInfo(
			bool show = true
		);

		// 分配控制台
		static void AllocConsole();

	protected:
		void Render();

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	protected:
		bool			end_;
		bool			inited_;
		float			time_scale_;
		String			app_name_;
		ScenePtr		curr_scene_;
		ScenePtr		next_scene_;
		NodePtr			debug_node_;
		TransitionPtr	transition_;
	};
}
