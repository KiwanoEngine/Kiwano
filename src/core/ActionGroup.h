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
#include "Action.hpp"

namespace easy2d
{
    // ѭ������
	class Loop
		: public Action
	{
	public:
		explicit Loop(
			ActionPtr const& action,	/* ִ��ѭ���Ķ��� */
			int times = -1			/* ѭ������ */
		);

		virtual ~Loop();

		// ��ȡ�ö����Ŀ�������
		virtual ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override;

		// ���ö���
		virtual void Reset() override;

		virtual bool IsRunning() override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration dt) override;

	protected:
		ActionPtr	action_;
		int			times_;
		int			total_times_;
	};


	// ˳����
	class Sequence
		: public Action
	{
	public:
		Sequence();

		explicit Sequence(
			Array<ActionPtr> const& actions	/* �����б� */
		);

		virtual ~Sequence();

		// �ڽ�β��Ӷ���
		void Add(
			ActionPtr const& action
		);

		// �ڽ�β��Ӷ������
		void Add(
			Array<ActionPtr> const& actions	/* �����б� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const override;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration dt) override;

	protected:
		UINT action_index_;
		Array<ActionPtr> actions_;
	};


	// ͬ������
	class Spawn
		: public Action
	{
	public:
		Spawn();

		explicit Spawn(
			Array<ActionPtr> const& actions	/* �����б� */
		);

		virtual ~Spawn();

		// �ڽ�β��Ӷ���
		void Add(
			ActionPtr const& action
		);

		// �ڽ�β��Ӷ������
		void Add(
			Array<ActionPtr> const& actions	/* �����б� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration dt) override;

	protected:
		Array<ActionPtr> actions_;
	};
}
