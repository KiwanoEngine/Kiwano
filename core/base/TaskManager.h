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
#include "Task.h"

namespace easy2d
{
	class TaskManager
	{
		using Tasks = intrusive::List<spTask>;

	public:
		// 添加任务
		void AddTask(
			spTask const& task
		);

		// 启动任务
		void StartTasks(
			std::wstring const& task_name
		);

		// 停止任务
		void StopTasks(
			std::wstring const& task_name
		);

		// 移除任务
		void RemoveTasks(
			std::wstring const& task_name
		);

		// 启动所有任务
		void StartAllTasks();

		// 停止所有任务
		void StopAllTasks();

		// 移除所有任务
		void RemoveAllTasks();

		// 获取所有任务
		const Tasks& GetAllTasks() const;

	protected:
		void UpdateTasks(Duration const& dt);

	protected:
		Tasks tasks_;
	};
}
