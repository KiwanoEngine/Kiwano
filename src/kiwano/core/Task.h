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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Time.h>
#include <kiwano/core/IntrusiveList.h>

namespace kiwano
{
class TaskManager;

KGE_DECLARE_SMART_PTR(Task);

/// \~chinese
/// @brief 任务列表
typedef IntrusiveList<TaskPtr> TaskList;

/// \~chinese
/// @brief 任务
/// @details 任务用于每隔一段时间执行一次回调函数，且可以指定执行总次数
class KGE_API Task
    : public ObjectBase
    , protected IntrusiveListValue<TaskPtr>
{
    friend class TaskManager;
    friend IntrusiveList<TaskPtr>;

public:
    /// \~chinese
    /// @brief 任务回调函数
    /// @details
    /// 回调函数第一个参数是任务自身，第二个参数是距离上次执行任务的时间间隔
    using Callback = Function<void(Task* /* self */, Duration /* dt */)>;

    /// \~chinese
    /// @brief 创建任务
    /// @param cb 回调函数
    /// @param interval 时间间隔
    /// @param times 执行次数（设 -1 为永久执行）
    static TaskPtr Create(const Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief 创建任务
    /// @param name 名称
    /// @param cb 回调函数
    /// @param interval 时间间隔
    /// @param times 执行次数（设 -1 为永久执行）
    static TaskPtr Create(const String& name, const Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief 构造空任务
    Task();

    /// \~chinese
    /// @brief 启动任务
    void Start();

    /// \~chinese
    /// @brief 停止任务
    void Stop();

    /// \~chinese
    /// @brief 移除任务
    void Remove();

    /// \~chinese
    /// @brief 任务是否在运行
    bool IsRunning() const;

    /// \~chinese
    /// @brief 任务是否可移除
    bool IsRemoveable() const;

    /// \~chinese
    /// @brief 获取任务执行过回调函数的次数
    int GetRunTimes() const;

    /// \~chinese
    /// @brief 获取任务执行回调函数的总次数
    int GetTotalRunTimes() const;

    /// \~chinese
    /// @brief 设置任务执行回调函数的总次数
    void SetTotalRunTimes(int times);

    /// \~chinese
    /// @brief 获取任务执行时间间隔
    Duration GetInterval() const;

    /// \~chinese
    /// @brief 设置任务执行时间间隔
    void SetInterval(Duration interval);

    /// \~chinese
    /// @brief 获取任务回调函数
    Callback GetCallback() const;

    /// \~chinese
    /// @brief 设置任务回调函数
    void SetCallback(const Callback& callback);

private:
    /// \~chinese
    /// @brief 更新任务
    void Update(Duration dt);

    /// \~chinese
    /// @brief 重置任务
    void Reset();

private:
    bool     running_;
    bool     removeable_;
    int      run_times_;
    int      total_times_;
    Duration interval_;
    Duration elapsed_;
    Callback callback_;
};

inline void Task::Start()
{
    running_ = true;
}

inline void Task::Stop()
{
    running_ = false;
}

inline void Task::Remove()
{
    removeable_ = true;
}

inline bool Task::IsRunning() const
{
    return running_;
}

inline bool Task::IsRemoveable() const
{
    return removeable_;
}

inline int Task::GetRunTimes() const
{
    return run_times_;
}

inline int Task::GetTotalRunTimes() const
{
    return total_times_;
}

inline void Task::SetTotalRunTimes(int times)
{
    total_times_ = times;
}

inline Duration Task::GetInterval() const
{
    return interval_;
}

inline void Task::SetInterval(Duration interval)
{
    interval_ = interval;
}

inline Task::Callback Task::GetCallback() const
{
    return callback_;
}

inline void Task::SetCallback(const Task::Callback& callback)
{
    callback_ = callback;
}
}  // namespace kiwano
