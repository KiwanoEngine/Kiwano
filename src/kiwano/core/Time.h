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
#include <kiwano/core/Duration.h>
#include <ctime>

namespace kiwano
{

/**
 * \~chinese
 * @brief 时间
 * @par 示例：
 * @code
 *   // 两时间相减, 可得到一个 Duration 对象
 *   Time t1 = Time::Now();
 *   // 等待一段时间后
 *   Time t2 = Time::Now();
 *   int ms = (t2 - t1).GetMilliseconds();  // 获取两时间相差的毫秒数
 * @endcode
 * @note 时间点与系统时钟无关，因此不能将时间点转化为时分秒
 */
struct KGE_API Time
{
    Time();

    /// \~chinese
    /// @brief 是否是零时
    /// @return 若是零时，返回true
    bool IsZero() const;

    /// \~chinese
    /// @brief 获取当前时间
    static Time Now() noexcept;

    const Duration operator-(const Time&) const;

    const Time operator+(const Duration&) const;
    const Time operator-(const Duration&) const;

    Time& operator+=(const Duration&);
    Time& operator-=(const Duration&);

private:
    Time(int64_t ms);

private:
    int64_t dur_;
};

/**
 * \~chinese
 * @brief 时钟时间
 */
struct KGE_API ClockTime
{
    ClockTime();

    /// \~chinese
    /// @brief 是否是零时
    /// @return 若是零时，返回true
    bool IsZero() const;

    /// \~chinese
    /// @brief 获取当前时间戳
    int64_t GetTimeStamp() const;

    /// \~chinese
    /// @brief 获取自纪元以来的毫秒数
    int64_t GetMillisecondsSinceEpoch() const;

    /// \~chinese
    /// @brief 获取 C 风格的时间
    std::time_t GetCTime() const;

    /// \~chinese
    /// @brief 获取当前时间
    static ClockTime Now() noexcept;

    /// \~chinese
    /// @brief 时间戳转化为时间
    static ClockTime FromTimeStamp(int64_t timestamp) noexcept;

    const Duration operator-(const ClockTime&) const;

    const ClockTime operator+(const Duration&) const;
    const ClockTime operator-(const Duration&) const;

    ClockTime& operator+=(const Duration&);
    ClockTime& operator-=(const Duration&);

private:
    ClockTime(int64_t ms_since_epoch);

private:
    int64_t ms_since_epoch_;
};

inline bool Time::IsZero() const
{
    return dur_ == 0;
}

inline bool ClockTime::IsZero() const
{
    return ms_since_epoch_ == 0;
}

}  // namespace kiwano
