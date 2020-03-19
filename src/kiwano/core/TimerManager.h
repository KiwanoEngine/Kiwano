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
#include <kiwano/core/Timer.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief 定时器管理器
 */
class KGE_API TimerManager
{
public:
    /// \~chinese
    /// @brief 添加定时器
    /// @param cb 回调函数
    /// @param interval 时间间隔
    /// @param times 执行次数（设 -1 为永久执行）
    Timer* AddTimer(const Timer::Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief 添加定时器
    /// @param name 定时器名称
    /// @param cb 回调函数
    /// @param interval 时间间隔
    /// @param times 执行次数（设 -1 为永久执行）
    Timer* AddTimer(const String& name, const Timer::Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief 添加定时器
    Timer* AddTimer(TimerPtr timer);

    /// \~chinese
    /// @brief 启动定时器
    void StartTimers(const String& timer_name);

    /// \~chinese
    /// @brief 停止定时器
    void StopTimers(const String& timer_name);

    /// \~chinese
    /// @brief 移除定时器
    void RemoveTimers(const String& timer_name);

    /// \~chinese
    /// @brief 启动所有定时器
    void StartAllTimers();

    /// \~chinese
    /// @brief 停止所有定时器
    void StopAllTimers();

    /// \~chinese
    /// @brief 移除所有定时器
    void RemoveAllTimers();

    /// \~chinese
    /// @brief 获取所有定时器
    const TimerList& GetAllTimers() const;

protected:
    /// \~chinese
    /// @brief 更新定时器
    void UpdateTimers(Duration dt);

private:
    TimerList timers_;
};
}  // namespace kiwano
