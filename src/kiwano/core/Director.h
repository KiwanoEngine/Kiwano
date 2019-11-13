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
#include <kiwano/2d/include-forwards.h>
#include <kiwano/core/Component.h>

namespace kiwano
{
	// 导演
	class KGE_API Director
		: public Singleton<Director>
		, public UpdateComponent
		, public RenderComponent
		, public EventComponent
	{
		KGE_DECLARE_SINGLETON(Director);

	public:
		// 切换舞台
		void EnterStage(
			StagePtr stage,						/* 舞台 */
			TransitionPtr transition = nullptr	/* 过渡动画 */
		);

		// 舞台压栈
		void PushStage(
			StagePtr stage,						/* 舞台 */
			TransitionPtr transition = nullptr	/* 过渡动画 */
		);

		// 舞台出栈
		void PopStage(
			TransitionPtr transition = nullptr	/* 过渡动画 */
		);

		// 获取当前舞台
		StagePtr GetCurrentStage();

		// 启用或禁用角色边界渲染功能
		void SetRenderBorderEnabled(bool enabled);

		// 显示调试信息
		void ShowDebugInfo(bool show = true);

		// 清空舞台
		void ClearStages();

	public:
		void SetupComponent() override {}

		void DestroyComponent() override {}

		void OnUpdate(Duration dt) override;

		void OnRender(RenderTarget* rt) override;

		void HandleEvent(Event* evt) override;

	protected:
		Director();

		virtual ~Director();

	protected:
		bool			render_border_enabled_;
		Stack<StagePtr>	stages_;
		StagePtr		current_stage_;
		StagePtr		next_stage_;
		ActorPtr		debug_actor_;
		TransitionPtr	transition_;
	};
}
