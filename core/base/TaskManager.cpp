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

#include "TaskManager.h"
#include "logs.h"

namespace easy2d
{
	void TaskManager::UpdateTasks(Duration const& dt)
	{
		if (tasks_.IsEmpty())
			return;

		spTask next;
		for (auto task = tasks_.First(); task; task = next)
		{
			next = task->NextItem();

			task->Update(dt);

			if (task->stopped_)
				tasks_.Remove(task);
		}
	}

	void TaskManager::AddTask(spTask const& task)
	{
		if (!task)
			logs::Warningln("Node::AddTask failed, action is nullptr");

		if (task)
		{
			task->Reset();
			tasks_.PushBack(Task::ItemType(task));
		}
	}

	void TaskManager::StopTasks(String const& name)
	{
		for (auto task = tasks_.First(); task; task = task->NextItem())
		{
			if (task->GetName() == name)
			{
				task->Stop();
			}
		}
	}

	void TaskManager::StartTasks(String const& name)
	{
		for (auto task = tasks_.First(); task; task = task->NextItem())
		{
			if (task->GetName() == name)
			{
				task->Start();
			}
		}
	}

	void TaskManager::RemoveTasks(String const& name)
	{
		for (auto task = tasks_.First(); task; task = task->NextItem())
		{
			if (task->GetName() == name)
			{
				task->stopped_ = true;
			}
		}
	}

	void TaskManager::StopAllTasks()
	{
		for (auto task = tasks_.First(); task; task = task->NextItem())
		{
			task->Stop();
		}
	}

	void TaskManager::StartAllTasks()
	{
		for (auto task = tasks_.First(); task; task = task->NextItem())
		{
			task->Start();
		}
	}

	void TaskManager::RemoveAllTasks()
	{
		for (auto task = tasks_.First(); task; task = task->NextItem())
		{
			task->stopped_ = true;
		}
	}

	const TaskManager::Tasks & TaskManager::GetAllTasks() const
	{
		return tasks_;
	}
}
