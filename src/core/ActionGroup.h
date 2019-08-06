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
	// ������
	class E2D_API ActionGroup
		: public Action
	{
	public:
		using ActionList = IntrusiveList<ActionPtr>;

		ActionGroup();

		explicit ActionGroup(
			Array<ActionPtr> const& actions,
			bool sequence = true				// ��˳��ִ�л�ͬʱִ��
		);

		virtual ~ActionGroup();

		// ��Ӷ���
		void Add(
			ActionPtr const& action
		);

		// ��Ӷ������
		void Add(
			Array<ActionPtr> const& actions
		);

		// ��ȡ���ж���
		inline ActionList const& GetActions() { return actions_; }

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
		bool		sequence_;
		ActionPtr	current_;
		ActionList	actions_;
	};


	// ˳����
	class E2D_DEPRECATED("ActionSequence is deprecated, use ActionGroup instead") E2D_API
		ActionSequence
		: public ActionGroup
	{
	public:
		ActionSequence();

		explicit ActionSequence(
			Array<ActionPtr> const& actions
		);

		virtual ~ActionSequence();
	};


	// ͬ������
	class E2D_DEPRECATED("ActionSpawn is deprecated, use ActionGroup instead") E2D_API
		ActionSpawn
		: public ActionGroup
	{
	public:
		ActionSpawn();

		explicit ActionSpawn(
			Array<ActionPtr> const& actions
		);

		virtual ~ActionSpawn();
	};
}
