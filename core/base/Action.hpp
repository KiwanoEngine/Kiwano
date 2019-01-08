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
#include "noncopyable.hpp"
#include "intrusive/List.hpp"

namespace easy2d
{
	class ActionManager;

	class Action
		: public Object
		, protected intrusive::ListItem<SpAction>
	{
		friend class ActionManager;
		friend class Loop;
		friend class Sequence;
		friend class Spawn;
		friend class intrusive::List<SpAction>;

	public:
		Action() : running_(false), done_(false), initialized_(false) {}

		virtual ~Action() {}

		// 获取动作运行状态
		virtual bool IsRunning() { return running_; }

		// 继续动作
		virtual void Resume() { running_ = true; }

		// 暂停动作
		virtual void Pause() { running_ = false; }

		// 停止动作
		virtual void Stop() { if (!done_) { done_ = true; if (cb_) cb_(); } }

		// 获取动作的拷贝
		virtual SpAction Clone() const = 0;

		// 获取动作的倒转
		virtual SpAction Reverse() const = 0;

		// 重置动作
		virtual void Reset()
		{
			initialized_ = false;
			done_ = false;
		}

		// 设置动作结束时的回调函数
		void SetCallback(std::function<void()> cb) { cb_ = cb; }

		virtual bool IsDone() const { return done_; }

	protected:
		virtual void Start()
		{
			running_ = true;
			this->Reset();
		}

		virtual void Init(Node* target) { initialized_ = true; }

		virtual void Update(Node* target, Duration const& dt)
		{
			if (!initialized_)
			{
				Init(target);
			}
		}

	protected:
		bool running_;
		bool done_;
		bool initialized_;
		std::function<void()> cb_;
	};
}
