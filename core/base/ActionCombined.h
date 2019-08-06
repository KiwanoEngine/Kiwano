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
#include "Action.h"

namespace easy2d
{
    // ѭ������
	class Loop
		: public Action
	{
		E2D_DISABLE_COPY(Loop);

	public:
		explicit Loop(
			Action * action,	/* ִ��ѭ���Ķ��� */
			int times = -1		/* ѭ������ */
		);

		virtual ~Loop();

		// ��ȡ�ö����Ŀ�������
		virtual Loop * Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual Loop * Reverse() const override;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Initialize() override;

		// ���¶���
		virtual void Update() override;

		// ���ö���ʱ��
		virtual void ResetTime() override;

	protected:
		Action * action_;
		int times_;
		int total_times_;
	};


	// ˳����
	class Sequence
		: public Action
	{
		E2D_DISABLE_COPY(Sequence);

	public:
		typedef std::vector<Action*> Actions;

		Sequence();

		explicit Sequence(
			const Actions& actions	/* �����б� */
		);

		virtual ~Sequence();

		// �ڽ�β��Ӷ���
		void Add(
			Action * action
		);

		// �ڽ�β��Ӷ������
		void Add(
			const Actions& actions	/* �����б� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual Sequence * Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual Sequence * Reverse() const;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Initialize() override;

		// ���¶���
		virtual void Update() override;

		// ���ö���ʱ��
		virtual void ResetTime() override;

	protected:
		UINT action_index_;
		Actions actions_;
	};


	// ͬ������
	class Spawn
		: public Action
	{
		E2D_DISABLE_COPY(Spawn);

	public:
		typedef std::vector<Action*> Actions;

		Spawn();

		explicit Spawn(
			const Actions& actions	/* �����б� */
		);

		virtual ~Spawn();

		// �ڽ�β��Ӷ���
		void Add(
			Action * action
		);

		// �ڽ�β��Ӷ������
		void Add(
			const Actions& actions	/* �����б� */
		);

		// ��ȡ�ö����Ŀ�������
		virtual Spawn * Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual Spawn * Reverse() const;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Initialize() override;

		// ���¶���
		virtual void Update() override;

		// ���ö���ʱ��
		virtual void ResetTime() override;

	protected:
		Actions actions_;
	};
}
