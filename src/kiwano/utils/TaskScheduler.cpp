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

#include <kiwano/utils/Logger.h>
#include <kiwano/utils/TaskScheduler.h>

namespace kiwano
{

void TaskScheduler::Update(Duration dt)
{
    if (tasks_.IsEmpty())
        return;

    TaskPtr next;
    for (auto task = tasks_.GetFirst(); task; task = next)
    {
        next = task->GetNext();

        task->Update(dt);

        if (task->IsRemoveable())
            tasks_.Remove(task);
    }
}

Task* TaskScheduler::AddTask(TaskPtr task)
{
    KGE_ASSERT(task && "AddTask failed, NULL pointer exception");

    if (task)
    {
        task->Reset();
        tasks_.PushBack(task);
    }
    return task.Get();
}

Task* TaskScheduler::AddTask(const Task::Callback& cb, TickerPtr ticker)
{
    auto task = MakePtr<Task>(cb, ticker);
    return AddTask(task);
}

Task* TaskScheduler::AddTask(StringView name, const Task::Callback& cb, TickerPtr ticker)
{
    auto task = MakePtr<Task>(name, cb, ticker);
    return AddTask(task);
}

Task* TaskScheduler::AddTask(const Task::Callback& cb, Duration interval, int times)
{
    auto task = MakePtr<Task>(cb, interval, times);
    return AddTask(task);
}

Task* TaskScheduler::AddTask(StringView name, const Task::Callback& cb, Duration interval, int times)
{
    auto task = MakePtr<Task>(name, cb, interval, times);
    return AddTask(task);
}

void TaskScheduler::StopTasks(StringView name)
{
    if (tasks_.IsEmpty())
        return;

    for (auto& task : tasks_)
    {
        if (task->IsName(name))
        {
            task->Stop();
        }
    }
}

void TaskScheduler::StartTasks(StringView name)
{
    if (tasks_.IsEmpty())
        return;

    for (auto& task : tasks_)
    {
        if (task->IsName(name))
        {
            task->Start();
        }
    }
}

void TaskScheduler::RemoveTasks(StringView name)
{
    if (tasks_.IsEmpty())
        return;

    for (auto& task : tasks_)
    {
        if (task->IsName(name))
        {
            task->Remove();
        }
    }
}

void TaskScheduler::StopAllTasks()
{
    if (tasks_.IsEmpty())
        return;

    for (auto& task : tasks_)
    {
        task->Stop();
    }
}

void TaskScheduler::StartAllTasks()
{
    if (tasks_.IsEmpty())
        return;

    for (auto& task : tasks_)
    {
        task->Start();
    }
}

void TaskScheduler::RemoveAllTasks()
{
    tasks_.Clear();
}

const TaskList& TaskScheduler::GetAllTasks() const
{
    return tasks_;
}
}  // namespace kiwano
