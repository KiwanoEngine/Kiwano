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
#include <kiwano/utils/Task.h>

namespace kiwano
{

/// \~chinese
/// @brief 任务列表
typedef IntrusiveList<TaskPtr> TaskList;

/**
 * \~chinese
 * @brief 任务调度器
 */
class KGE_API TaskScheduler : Noncopyable
{
public:
    /// \~chinese
    /// @brief 添加任务
    Task* AddTask(TaskPtr task);

    /// \~chinese
    /// @brief 添加任务
    /// @param cb 回调函数
    /// @param 报时器
    Task* AddTask(const Task::Callback& cb, TickerPtr ticker);

    /// \~chinese
    /// @brief 添加任务
    /// @param name 名称
    /// @param cb 回调函数
    /// @param 报时器
    Task* AddTask(const String& name, const Task::Callback& cb, TickerPtr ticker);

    /// \~chinese
    /// @brief 添加任务
    /// @param cb 回调函数
    /// @param interval 时间间隔
    /// @param times 执行次数（设 -1 为永久执行）
    Task* AddTask(const Task::Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief 添加任务
    /// @param name 名称
    /// @param cb 回调函数
    /// @param interval 时间间隔
    /// @param times 执行次数（设 -1 为永久执行）
    Task* AddTask(const String& name, const Task::Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief 启动任务
    void StartTasks(const String& task_name);

    /// \~chinese
    /// @brief 停止任务
    void StopTasks(const String& task_name);

    /// \~chinese
    /// @brief 移除任务
    void RemoveTasks(const String& task_name);

    /// \~chinese
    /// @brief 启动所有任务
    void StartAllTasks();

    /// \~chinese
    /// @brief 停止所有任务
    void StopAllTasks();

    /// \~chinese
    /// @brief 移除所有任务
    void RemoveAllTasks();

    /// \~chinese
    /// @brief 获取所有任务
    const TaskList& GetAllTasks() const;

    /// \~chinese
    /// @brief 更新调度器
    void Update(Duration dt);

private:
    TaskList tasks_;
};

}  // namespace kiwano
