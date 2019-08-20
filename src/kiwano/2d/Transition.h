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
#include "include-forwards.h"
#include "../renderer/LayerArea.h"

namespace kiwano
{
	class Director;
	class RenderTarget;

	// ��̨����
	class KGE_API Transition
		: public ObjectBase
	{
		friend class Director;

	public:
		explicit Transition(
			Duration duration
		);

		virtual ~Transition();

		bool IsDone();

	protected:
		virtual void Init(
			StagePtr prev,
			StagePtr next
		);

		virtual void Update(Duration dt);

		virtual void Render(RenderTarget* rt);

		virtual void Stop();

		virtual void Reset() { };

	protected:
		bool		done_;
		Float32		process_;
		Duration	duration_;
		Duration	delta_;
		Size		window_size_;
		StagePtr	out_stage_;
		StagePtr	in_stage_;
		LayerArea	out_layer_;
		LayerArea	in_layer_;
	};


	// ���뵭������
	class FadeTransition
		: public Transition
	{
	public:
		explicit FadeTransition(
			Duration duration	/* ��������ʱ�� */
		);

	protected:
		// ���¶���
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;
	};


	// �������
	class EmergeTransition
		: public Transition
	{
	public:
		explicit EmergeTransition(
			Duration duration	/* ��������ʱ�� */
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;
	};


	// ��״����
	class BoxTransition
		: public Transition
	{
	public:
		explicit BoxTransition(
			Duration duration	/* ��������ʱ�� */
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;
	};


	// λ�ƹ���
	class MoveTransition
		: public Transition
	{
	public:
		enum class Type : Int32
		{
			Up,		/* ���� */
			Down,	/* ���� */
			Left,	/* ���� */
			Right	/* ���� */
		};

		explicit MoveTransition(
			Duration duration,	/* ��������ʱ�� */
			Type type			/* �ƶ���ʽ */
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;

		void Reset() override;

	protected:
		Type	type_;
		Point	pos_delta_;
		Point	start_pos_;
	};


	// ��ת����
	class RotationTransition
		: public Transition
	{
	public:
		explicit RotationTransition(
			Duration duration,		/* ��������ʱ�� */
			Float32 rotation = 360	/* ��ת���� */
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;

		void Reset() override;

	protected:
		Float32	rotation_;
	};
}
