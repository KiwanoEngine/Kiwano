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

namespace easy2d
{
	class Scene;

	// ��������
	class E2D_API Transition
		: public virtual Object
	{
		friend class Application;

	public:
		explicit Transition(
			Duration duration
		);

		virtual ~Transition();

		bool IsDone();

	protected:
		virtual void Init(
			ScenePtr const& prev,
			ScenePtr const& next
		);

		virtual void Update(Duration dt);

		virtual void Render();

		virtual void Stop();

		virtual void Reset() { };

	protected:
		bool			done_;
		float			process_;
		Duration		duration_;
		Duration		delta_;
		Size			window_size_;
		ScenePtr			out_scene_;
		ScenePtr			in_scene_;
		D2DLayerPtr			out_layer_;
		D2DLayerPtr			in_layer_;
		LayerProperties out_layer_prop_;
		LayerProperties in_layer_prop_;
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
			ScenePtr const& prev,
			ScenePtr const& next
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
			ScenePtr const& prev,
			ScenePtr const& next
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
			ScenePtr const& prev,
			ScenePtr const& next
		) override;
	};


	// λ�ƹ���
	class MoveTransition
		: public Transition
	{
	public:
		explicit MoveTransition(
			Duration moveDuration,	/* ��������ʱ�� */
			Direction direction				/* �ƶ����� */
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			ScenePtr const& prev,
			ScenePtr const& next
		) override;

		void Reset() override;

	protected:
		Direction	direction_;
		Point		pos_delta_;
		Point		start_pos_;
	};


	// ��ת����
	class RotationTransition
		: public Transition
	{
	public:
		explicit RotationTransition(
			Duration moveDuration,	/* ��������ʱ�� */
			float rotation = 360			/* ��ת���� */
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			ScenePtr const& prev,
			ScenePtr const& next
		) override;

		void Reset() override;

	protected:
		float	rotation_;
	};
}
