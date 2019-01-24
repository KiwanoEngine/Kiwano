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
#include <functional>

namespace easy2d
{
	class TaskManager;

    // 定时任务
	class Task
		: public virtual Object
		, protected IntrusiveListItem<TaskPtr>
	{
		friend class TaskManager;
		friend class IntrusiveList<TaskPtr>;

		using Callback = std::function<void()>;

	public:
		explicit Task(
			Callback const& func,			/* 执行函数 */
			String const& name = L""	/* 任务名称 */
		);

		explicit Task(
			Callback const& func,			/* 执行函数 */
			Duration const& delay,			/* 时间间隔（秒） */
			int times = -1,					/* 执行次数（设 -1 为永久执行） */
			String const& name = L""	/* 任务名称 */
		);

		// 启动任务
		void Start();

		// 停止任务
		void Stop();

		// 任务是否正在执行
		bool IsRunning() const;

		// 获取任务名称
		String const& GetName() const;

	protected:
		void Update(Duration const& dt, bool& remove_after_update);

		void Reset();

	protected:
		bool			running_;
		int				run_times_;
		int				total_times_;
		String	name_;
		Duration		delay_;
		Duration		delta_;
		Callback		callback_;
	};
}
