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
#include "IntrusiveList.hpp"

namespace easy2d
{
	using ActionCallback = std::function<void()>;

	class ActionManager;

	class Action
		: public virtual Object
		, protected IntrusiveListItem<ActionPtr>
	{
		friend class ActionManager;
		friend class Loop;
		friend class Sequence;
		friend class Spawn;
		friend class IntrusiveList<ActionPtr>;

	public:
		enum class Status
		{
			NotStarted,
			Delayed,
			Started,
			Done,
			Removeable
		};

		Action();

		virtual ~Action();

		// ��������
		inline void Resume()										{ running_ = true; }

		// ��ͣ����
		inline void Pause()											{ running_ = false; }

		// ֹͣ����
		inline void Stop()											{ status_ = Status::Removeable; }

		// ���ö�����ʱ
		inline void SetDelay(Duration delay)						{ delay_ = delay; }

		// ����ѭ������ (-1 Ϊ����ѭ��)
		inline void SetLoops(int loops)								{ loops_ = loops; }

		// ��������ʱ�Ƴ�Ŀ��ڵ�
		inline void RemoveTargetWhenDone()							{ detach_target_ = true; }

		// ���ö�������ʱ�Ļص�����
		inline void SetDoneCallback(ActionCallback const& cb)		{ cb_done_ = cb; }

		// ���ö���ѭ������ʱ�Ļص�����
		inline void SetLoopDoneCallback(ActionCallback const& cb)	{ cb_loop_done_ = cb; }

		// ��ȡ�����Ŀ���
		virtual ActionPtr Clone() const = 0;

		// ��ȡ�����ĵ�ת
		virtual ActionPtr Reverse() const = 0;

		inline void Done()											{ status_ = Status::Done; }

		inline Status GetStatus() const								{ return status_; }

		inline bool IsRunning() const								{ return running_; }

		inline bool IsDone() const									{ return status_ == Status::Done || status_ == Status::Removeable; }

		inline bool IsRemoveable() const							{ return status_ == Status::Removeable; }

		inline int GetLoops() const									{ return loops_; }

		inline Duration GetDelay() const							{ return delay_; }

		inline Duration GetElapsed() const							{ return elapsed_; }

		inline ActionCallback const& GetDoneCallback() const		{ return cb_done_; }

		inline ActionCallback const& GetLoopDoneCallback() const	{ return cb_loop_done_; }

	protected:
		virtual void Init(NodePtr const& target) {}

		virtual void Update(NodePtr const& target, Duration dt) {}

		void UpdateStep(NodePtr const& target, Duration dt);

		void Complete(NodePtr const& target);

		void Restart(NodePtr const& target);

	protected:
		Status			status_;
		bool			running_;
		bool			detach_target_;
		int				loops_;
		int				loops_done_;
		Duration		delay_;
		Duration		elapsed_;
		ActionCallback	cb_done_;
		ActionCallback	cb_loop_done_;
	};
}
