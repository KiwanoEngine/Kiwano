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
#include <kiwano/2d/action/Action.h>

namespace kiwano
{
	// 动作组
	class KGE_API ActionGroup
		: public Action
	{
	public:
		using ActionList = IntrusiveList<ActionPtr>;

		ActionGroup();

		explicit ActionGroup(
			Vector<ActionPtr> const& actions,
			bool sequence = true				// 按顺序执行或同时执行
		);

		virtual ~ActionGroup();

		// 添加动作
		void Add(
			ActionPtr action
		);

		// 添加多个动作
		void Add(
			Vector<ActionPtr> const& actions
		);

		// 获取所有动作
		inline ActionList const& GetActions() { return actions_; }

		// 获取该动作的拷贝对象
		ActionPtr Clone() const override;

		// 获取该动作的倒转
		ActionPtr Reverse() const override;

	protected:
		// 初始化动作
		void Init(ActorPtr target) override;

		// 更新动作
		void Update(ActorPtr target, Duration dt) override;

	protected:
		bool		sequence_;
		ActionPtr	current_;
		ActionList	actions_;
	};


#pragma warning(push)
#pragma warning(disable : 4996)

	// 顺序动作
	class KGE_API ActionSequence
		: public ActionGroup
	{
	public:
		KGE_DEPRECATED("ActionSequence is deprecated, use ActionGroup instead")
		inline ActionSequence() : ActionGroup() {}

		KGE_DEPRECATED("ActionSequence is deprecated, use ActionGroup instead")
		inline explicit ActionSequence(Vector<ActionPtr> const& actions) : ActionGroup(actions, true) {}

		virtual ~ActionSequence() {}
	};


	// 同步动作
	class KGE_API ActionSpawn
		: public ActionGroup
	{
	public:
		KGE_DEPRECATED("ActionSpawn is deprecated, use ActionGroup instead")
		inline ActionSpawn() : ActionGroup() { sequence_ = false; }

		KGE_DEPRECATED("ActionSpawn is deprecated, use ActionGroup instead")
		inline explicit ActionSpawn(Vector<ActionPtr> const& actions) : ActionGroup(actions, false) {}

		virtual ~ActionSpawn() {}
	};

#pragma warning(pop)

}
