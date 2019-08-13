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
#include "../macros.h"
#include "../2d/include-forwards.h"
#include "Component.h"

namespace kiwano
{
	// ����
	class KGE_API Director
		: public Singleton<Director>
		, public Component
	{
		KGE_DECLARE_SINGLETON(Director);

	public:
		// �л���̨
		void EnterStage(
			StagePtr scene				/* ��̨ */
		);

		// �л���̨
		void EnterStage(
			StagePtr scene,				/* ��̨ */
			TransitionPtr transition	/* ���ɶ��� */
		);

		// ��ȡ��ǰ��̨
		StagePtr GetCurrentStage();

		// ���û���ý�ɫ�߽���Ⱦ����
		void SetRenderBorderEnabled(bool enabled);

		// ��ʾ������Ϣ
		void ShowDebugInfo(bool show = true);

		// �����̨
		void ClearStages();

	public:
		void SetupComponent() override {}

		void DestroyComponent() override {}

		void OnUpdate(Duration dt) override;

		void OnRender() override;

		void AfterRender() override;

		void HandleEvent(Event& evt) override;

	protected:
		Director();

		virtual ~Director();

	protected:
		bool			render_border_enabled_;
		StagePtr		curr_scene_;
		StagePtr		next_scene_;
		ActorPtr		debug_actor_;
		TransitionPtr	transition_;
	};
}
