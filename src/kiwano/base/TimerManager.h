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
#include <kiwano/base/Timer.h>

namespace kiwano
{
	class KGE_API TimerManager
	{
		using Timers = IntrusiveList<TimerPtr>;

	public:
		// 添加定时器
		Timer* AddTimer(
			Timer::Callback const& func,	/* 执行函数 */
			Duration delay,					/* 时间间隔（秒） */
			int times = -1,				/* 执行次数（设 -1 为永久执行） */
			String const& name = L""		/* 任务名称 */
		);

		// 添加定时器
		Timer* AddTimer(
			TimerPtr timer
		);

		// 启动任务
		void StartTimers(
			String const& timer_name
		);

		// 停止任务
		void StopTimers(
			String const& timer_name
		);

		// 移除任务
		void RemoveTimers(
			String const& timer_name
		);

		// 启动所有任务
		void StartAllTimers();

		// 停止所有任务
		void StopAllTimers();

		// 移除所有任务
		void RemoveAllTimers();

		// 获取所有任务
		const Timers& GetAllTimers() const;

	protected:
		void UpdateTimers(Duration dt);

	protected:
		Timers timers_;
	};
}
