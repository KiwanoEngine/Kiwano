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
	// ˳����
	class E2D_API Sequence
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
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		// ��ʼ������
		void Init(NodePtr const& target) override;

		// ���¶���
		void Update(NodePtr const& target, Duration dt) override;

	protected:
		ActionPtr current_;
		IntrusiveList<ActionPtr> actions_;
	};


	// ͬ������
	class E2D_API Spawn
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
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual ActionPtr Reverse() const;

	protected:
		// ��ʼ������
		void Init(NodePtr const& target) override;

		// ���¶���
		void Update(NodePtr const& target, Duration dt) override;

	protected:
		int size_;
		IntrusiveList<ActionPtr> actions_;
	};
}
