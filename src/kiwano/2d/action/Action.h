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
#include <kiwano/2d/include-forwards.h>

namespace kiwano
{
	using ActionCallback = Function<void()>;

	class ActionManager;

	class KGE_API Action
		: public ObjectBase
		, protected IntrusiveListItem<ActionPtr>
	{
		friend class ActionManager;
		friend class ActionGroup;
		friend IntrusiveList<ActionPtr>;

	public:
		Action();

		virtual ~Action();

		// 继续动作
		void Resume();

		// 暂停动作
		void Pause();

		// 停止动作
		void Stop();

		// 设置动作延时
		void SetDelay(Duration delay);

		// 设置循环次数 (-1 为永久循环)
		void SetLoops(int loops);

		// 动作结束时移除目标角色
		void RemoveTargetWhenDone();

		// 设置动作结束时的回调函数
		void SetDoneCallback(ActionCallback const& cb);

		// 设置动作循环结束时的回调函数
		void SetLoopDoneCallback(ActionCallback const& cb);

		// 获取动作的拷贝
		virtual ActionPtr Clone() const = 0;

		// 获取动作的倒转
		virtual ActionPtr Reverse() const = 0;

		bool IsRunning() const;

		int GetLoops() const;

		Duration GetDelay() const;

		ActionCallback GetDoneCallback() const;

		ActionCallback GetLoopDoneCallback() const;

	protected:
		virtual void Init(ActorPtr target);

		virtual void Update(ActorPtr target, Duration dt);

		void UpdateStep(ActorPtr target, Duration dt);

		void Complete(ActorPtr target);

		void Restart(ActorPtr target);

		enum class Status
		{
			NotStarted,
			Delayed,
			Started,
			Done,
			Removeable
		};

		Status GetStatus() const;

		Duration GetElapsed() const;

		int GetLoopsDone() const;

		void Done();

		bool IsDone() const;

		bool IsRemoveable() const;

	private:
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


	inline void Action::Resume()
	{
		running_ = true;
	}

	inline void Action::Pause()
	{
		running_ = false;
	}

	inline void Action::Stop()
	{
		Done();
	}

	inline void Action::SetDelay(Duration delay)
	{
		delay_ = delay;
	}

	inline void Action::SetLoops(int loops)
	{
		loops_ = loops;
	}

	inline void Action::RemoveTargetWhenDone()
	{
		detach_target_ = true;
	}

	inline void Action::SetDoneCallback(ActionCallback const& cb)
	{
		cb_done_ = cb;
	}

	inline void Action::SetLoopDoneCallback(ActionCallback const& cb)
	{
		cb_loop_done_ = cb;
	}

	inline void Action::Done()
	{
		status_ = Status::Done;
	}

	inline Action::Status Action::GetStatus() const
	{
		return status_;
	}

	inline bool Action::IsRunning() const
	{
		return running_;
	}

	inline bool Action::IsDone() const
	{
		return status_ == Status::Done || status_ == Status::Removeable;
	}

	inline bool Action::IsRemoveable() const
	{
		return status_ == Status::Removeable;
	}

	inline int Action::GetLoops() const
	{
		return loops_;
	}

	inline Duration Action::GetDelay() const
	{
		return delay_;
	}

	inline Duration Action::GetElapsed() const
	{
		return elapsed_;
	}

	inline int Action::GetLoopsDone() const
	{
		return loops_done_;
	}

	inline ActionCallback Action::GetDoneCallback() const
	{
		return cb_done_;
	}

	inline ActionCallback Action::GetLoopDoneCallback() const
	{
		return cb_loop_done_;
	}
}
