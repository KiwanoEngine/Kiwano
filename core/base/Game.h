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
#include "window.h"
#include "Scene.h"
#include "Transition.h"

namespace easy2d
{
	struct Options
	{
		String	title;	/* 标题 */
		int		width;	/* 宽度 */
		int		height;	/* 高度 */
		LPCWSTR	icon;	/* 图标 */
		bool	debug;	/* 调试模式 */

		Options()
			: title(L"Easy2D Game")
			, width(640)
			, height(480)
			, icon(nullptr)
			, debug(false)
		{}
	};


	class Game
	{
		E2D_DISABLE_COPY(Game);

	public:
		Game();

		virtual ~Game();

		// 更新时
		virtual void OnUpdate(float dt) {}

		// 退出时
		virtual void OnExit() {}

		// 窗口关闭时
		// 返回值：返回 false 将阻止窗口关闭
		virtual bool OnClose() { return true; }

		// 初始化
		void Init(
			const Options& options	/* 属性 */
		);

		// 运行
		void Run();

		// 结束
		void Quit();

		// 切换场景
		void EnterScene(
			spScene const& scene,						/* 场景 */
			spTransition const& transition = nullptr	/* 场景动画 */
		);

		// 获取当前场景
		spScene const& GetCurrentScene();

		// 是否正在进行场景过渡
		bool IsTransitioning() const;

		// 渲染场景画面
		void DrawScene();

		// 更新场景
		void UpdateScene(
			float dt
		);

	private:
		bool			debug_mode_;
		bool			quit_;
		spScene			curr_scene_;
		spScene			next_scene_;
		spTransition	transition_;
	};
}
