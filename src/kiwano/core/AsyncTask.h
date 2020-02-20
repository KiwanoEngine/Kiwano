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
#include <mutex>
#include <thread>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(AsyncTask);

typedef Function<void()> AsyncTaskFunc;
typedef Function<void()> AsyncTaskCallback;

/// \~chinese
/// @brief 异步任务
/// @details 在多线程下执行任务并返回
///   @code
///     AsyncTaskPtr task = new AsyncTask;
///     task->Then(DoSomething);
///     task->Start();
///   @endcode
class KGE_API AsyncTask : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 创建异步任务
    /// @param func 异步回调函数
    static AsyncTaskPtr Create(AsyncTaskFunc func);

    /// \~chinese
    /// @brief 构造异步任务
    AsyncTask();

    virtual ~AsyncTask();

    /// \~chinese
    /// @brief 添加异步任务链
    AsyncTask& Then(AsyncTaskFunc func);

    /// \~chinese
    /// @brief 设置任务执行完成后的回调函数
    /// @note 该函数在 Kiwano 主线程中执行
    AsyncTask& SetCallback(AsyncTaskCallback callback);

    /// \~chinese
    /// @brief 启动异步任务
    void Start();

private:
    void TaskThread();

    void Complete();

private:
    std::thread          thread_;
    std::mutex           func_mutex_;
    Queue<AsyncTaskFunc> thread_func_queue_;
    AsyncTaskCallback    thread_cb_;
};
}  // namespace kiwano
