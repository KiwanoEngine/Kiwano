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
#include <kiwano/core/Common.h>
#include <ctime>

namespace kiwano
{

/**
 * \~chinese
 * @brief 时间段
 * @par
 *   时间段表示法:
 *   @code
 *     Duration::Ms * 50     // 50 毫秒
 *     Duration::Second * 5  // 5 秒
 *     Duration::Hour * 1.5  // 1.5 小时
 *     Duration::Hour * 3 + Duration::Minute * 45 + Duration::Second * 15  // 3 小时 45 分 15秒
 *   @endcode
 *   在 VS2015 及更高版本可以使用 time literals:
 *   @code
 *     using namespace kiwano;
 *     50_msec                   // 50 毫秒
 *     5_sec                     // 5 秒
 *     1.5_hour                  // 1.5 小时
 *     3_hour + 45_min + 15_sec  // 3 小时 45 分 15 秒
 *   @endcode
 */
struct KGE_API Duration
{
    /// \~chinese
    /// @brief 构造时间段
    Duration();

    /// \~chinese
    /// @brief 构造时间段
    /// @param milliseconds 毫秒数
    Duration(int64_t milliseconds);

    /// \~chinese
    /// @brief 获取毫秒数
    int64_t GetMilliseconds() const;

    /// \~chinese
    /// @brief 获取秒数
    float GetSeconds() const;

    /// \~chinese
    /// @brief 获取分钟数
    float GetMinutes() const;

    /// \~chinese
    /// @brief 获取小时数
    float GetHours() const;

    /// \~chinese
    /// @brief 时长是否是零
    /// @return 若时长是零，返回true
    bool IsZero() const;

    /// \~chinese
    /// @brief 设置毫秒数
    /// @param ms 毫秒数
    void SetMilliseconds(int64_t ms);

    /// \~chinese
    /// @brief 设置秒数
    /// @param seconds 秒数
    void SetSeconds(float seconds);

    /// \~chinese
    /// @brief 设置分钟数
    /// @param minutes 分钟数
    void SetMinutes(float minutes);

    /// \~chinese
    /// @brief 设置小时数
    /// @param hours 小时数
    void SetHours(float hours);

    /// \~chinese
    /// @brief 休眠
    void Sleep() const;

    /// \~chinese
    /// @brief 转为字符串
    String ToString() const;

    /// \~chinese
    /// @brief 解析时间段字符串
    /// @param str 时间段字符串
    /// @details
    ///   时间段字符串允许是有符号的浮点数, 并且带有时间单位后缀
    ///   例如: "300ms", "-1.5h", "2h45m"
    ///   允许的时间单位有 "ms", "s", "m", "h"
    /// @return 解析出的时间段
    /// @throw kiwano::RuntimeError 传入一个不合法的格式时抛出
    static Duration Parse(const String& str);

    static const Duration Ms;      ///< 毫秒
    static const Duration Second;  ///< 秒
    static const Duration Minute;  ///< 分钟
    static const Duration Hour;    ///< 小时

    bool operator==(const Duration&) const;
    bool operator!=(const Duration&) const;
    bool operator>(const Duration&) const;
    bool operator>=(const Duration&) const;
    bool operator<(const Duration&) const;
    bool operator<=(const Duration&) const;

    float operator/(const Duration&) const;

    const Duration operator+(const Duration&) const;
    const Duration operator-(const Duration&) const;
    const Duration operator-() const;
    const Duration operator*(int)const;
    const Duration operator*(unsigned long long)const;
    const Duration operator*(float)const;
    const Duration operator*(double)const;
    const Duration operator*(long double)const;
    const Duration operator/(int) const;
    const Duration operator/(float) const;
    const Duration operator/(double) const;

    Duration& operator+=(const Duration&);
    Duration& operator-=(const Duration&);
    Duration& operator*=(int);
    Duration& operator*=(float);
    Duration& operator*=(double);
    Duration& operator/=(int);
    Duration& operator/=(float);
    Duration& operator/=(double);

    friend const Duration operator*(int, const Duration&);
    friend const Duration operator*(float, const Duration&);
    friend const Duration operator*(double, const Duration&);
    friend const Duration operator*(long double, const Duration&);
    friend const Duration operator/(int, const Duration&);
    friend const Duration operator/(float, const Duration&);
    friend const Duration operator/(double, const Duration&);

private:
    int64_t milliseconds_;
};

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

inline int64_t Duration::GetMilliseconds() const
{
    return milliseconds_;
}

inline bool Duration::IsZero() const
{
    return milliseconds_ == 0LL;
}

inline void Duration::SetMilliseconds(int64_t ms)
{
    milliseconds_ = ms;
}

inline void Duration::SetSeconds(float seconds)
{
    milliseconds_ = static_cast<int64_t>(seconds * 1000.f);
}

inline void Duration::SetMinutes(float minutes)
{
    milliseconds_ = static_cast<int64_t>(minutes * 60 * 1000.f);
}

inline void Duration::SetHours(float hours)
{
    milliseconds_ = static_cast<int64_t>(hours * 60 * 60 * 1000.f);
}

inline bool Time::IsZero() const
{
    return dur_ == 0;
}

}  // namespace kiwano

#if defined(KGE_HAS_LITERALS)

namespace kiwano
{
inline namespace literals
{
inline const kiwano::Duration operator"" _msec(long double val)
{
    return kiwano::Duration::Ms * val;
}

inline const kiwano::Duration operator"" _msec(unsigned long long val)
{
    return kiwano::Duration::Ms * val;
}

inline const kiwano::Duration operator"" _sec(long double val)
{
    return kiwano::Duration::Second * val;
}

inline const kiwano::Duration operator"" _sec(unsigned long long val)
{
    return kiwano::Duration::Second * val;
}

inline const kiwano::Duration operator"" _min(long double val)
{
    return kiwano::Duration::Minute * val;
}

inline const kiwano::Duration operator"" _min(unsigned long long val)
{
    return kiwano::Duration::Minute * val;
}

inline const kiwano::Duration operator"" _hour(long double val)
{
    return kiwano::Duration::Hour * val;
}

inline const kiwano::Duration operator"" _hour(unsigned long long val)
{
    return kiwano::Duration::Hour * val;
}
}  // namespace literals
}  // namespace kiwano

#endif
