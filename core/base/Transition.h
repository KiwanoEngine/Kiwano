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
#include "time.h"
#include "RefCounter.h"

namespace easy2d
{
	class Game;
	class Scene;

	// ��������
	class Transition
		: public RefCounter
	{
		friend class Game;

	public:
		explicit Transition(
			float duration
		);

		virtual ~Transition();

		// �������ɶ����Ƿ����
		bool IsDone();

	protected:
		// ��ʼ���������ɶ���
		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		);

		// ���³������ɶ���
		virtual void Update();

		// ��Ⱦ�������ɶ���
		virtual void Draw();

		// ֹͣ�������ɶ���
		virtual void Stop();

		// ���ó������ɶ���
		virtual void Reset() { };

	protected:
		bool	done_;
		float	duration_;
		float	process_;
		time::TimePoint	started_;
		Size	window_size_;
		Scene*	out_scene_;
		Scene*	in_scene_;
		ID2D1Layer * out_layer_;
		ID2D1Layer * in_layer_;
		D2D1_LAYER_PARAMETERS out_layer_param_;
		D2D1_LAYER_PARAMETERS in_layer_param_;
	};


	// ���뵭������
	class FadeTransition
		: public Transition
	{
	public:
		explicit FadeTransition(
			float duration		/* ��������ʱ�� */
		);

	protected:
		// ���¶���
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;
	};


	// �������
	class EmergeTransition
		: public Transition
	{
	public:
		explicit EmergeTransition(
			float duration		/* ��������ʱ�� */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;
	};


	// ��״����
	class BoxTransition
		: public Transition
	{
	public:
		explicit BoxTransition(
			float duration		/* ��������ʱ�� */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;
	};


	// λ�ƹ���
	class MoveTransition
		: public Transition
	{
	public:
		explicit MoveTransition(
			float moveDuration,						/* ��������ʱ�� */
			Direction direction = Direction::Left	/* �ƶ����� */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;

		virtual void Reset() override;

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
			float moveDuration,		/* ��������ʱ�� */
			float rotation = 360	/* ��ת���� */
		);

	protected:
		virtual void Update() override;

		virtual void Initialize(
			Scene * prev,
			Scene * next,
			Game * game
		) override;

		virtual void Reset() override;

	protected:
		float	rotation_;
	};
}
