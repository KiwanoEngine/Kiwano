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
#include <kiwano/utils/Timer.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Ticker);

/// \~chinese
/// @brief 报时器
class KGE_API Ticker
    : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 创建报时器
    /// @param interval 报时间隔
    /// @param times 报时次数（设 -1 为永久）
    static TickerPtr Create(Duration interval, int times = -1);

    Ticker();

    /// \~chinese
    /// @brief 计时
    /// @return 是否达到报时时长
    virtual bool Tick();

    /// \~chinese
    /// @brief 计时
    /// @param dt 时间增量
    /// @return 是否达到报时时长
    virtual bool Tick(Duration dt);

    /// \~chinese
    /// @brief 获取时间增量
    Duration GetDeltaTime();

    /// \~chinese
    /// @brief 获取暂停状态
    bool IsPausing() const;

    /// \~chinese
    /// @brief 暂停报时器
    void Pause();

    /// \~chinese
    /// @brief 继续报时器
    void Resume();

    /// \~chinese
    /// @brief 获取报时器报时次数
    int GetTickedTimes() const;

    /// \~chinese
    /// @brief 获取报时器总报时次数
    int GetTotalTickTimes() const;

    /// \~chinese
    /// @brief 设置报时器总报时次数
    void SetTotalTickTimes(int times);

    /// \~chinese
    /// @brief 获取报时间隔
    Duration GetInterval() const;

    /// \~chinese
    /// @brief 设置报时间隔
    void SetInterval(Duration interval);

    /// \~chinese
    /// @brief 获取计时器
    TimerPtr GetTimer();

    /// \~chinese
    /// @brief 设置计时器
    void SetTimer(TimerPtr timer);

    /// \~chinese
    /// @brief 重置报时器
    void Reset();

private:
    bool     is_paused_;
    int      ticked_times_;
    int      total_times_;
    Duration interval_;
    Duration elapsed_time_;
    Duration delta_time_;
    Duration error_time_;
    TimerPtr timer_;
};

inline bool Ticker::IsPausing() const
{
    return is_paused_;
}

inline int Ticker::GetTickedTimes() const
{
    return ticked_times_;
}

inline int Ticker::GetTotalTickTimes() const
{
    return total_times_;
}

inline void Ticker::SetTotalTickTimes(int times)
{
    total_times_ = times;
}

inline Duration Ticker::GetInterval() const
{
    return interval_;
}

inline void Ticker::SetInterval(Duration interval)
{
    interval_ = interval;
}

}  // namespace kiwano
