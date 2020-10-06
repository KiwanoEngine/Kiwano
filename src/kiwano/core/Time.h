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
 * @brief ʱ���
 * @par
 *   ʱ��α�ʾ��:
 *   @code
 *     Duration::Ms * 50     // 50 ����
 *     Duration::Second * 5  // 5 ��
 *     Duration::Hour * 1.5  // 1.5 Сʱ
 *     Duration::Hour * 3 + Duration::Minute * 45 + Duration::Second * 15  // 3 Сʱ 45 �� 15��
 *   @endcode
 *   �� VS2015 �����߰汾����ʹ�� time literals:
 *   @code
 *     using namespace kiwano;
 *     50_msec                   // 50 ����
 *     5_sec                     // 5 ��
 *     1.5_hour                  // 1.5 Сʱ
 *     3_hour + 45_min + 15_sec  // 3 Сʱ 45 �� 15 ��
 *   @endcode
 */
struct KGE_API Duration
{
    /// \~chinese
    /// @brief ����ʱ���
    Duration();

    /// \~chinese
    /// @brief ����ʱ���
    /// @param milliseconds ������
    Duration(int64_t milliseconds);

    /// \~chinese
    /// @brief ��ȡ������
    int64_t GetMilliseconds() const;

    /// \~chinese
    /// @brief ��ȡ����
    float GetSeconds() const;

    /// \~chinese
    /// @brief ��ȡ������
    float GetMinutes() const;

    /// \~chinese
    /// @brief ��ȡСʱ��
    float GetHours() const;

    /// \~chinese
    /// @brief ʱ���Ƿ�����
    /// @return ��ʱ�����㣬����true
    bool IsZero() const;

    /// \~chinese
    /// @brief ���ú�����
    /// @param ms ������
    void SetMilliseconds(int64_t ms);

    /// \~chinese
    /// @brief ��������
    /// @param seconds ����
    void SetSeconds(float seconds);

    /// \~chinese
    /// @brief ���÷�����
    /// @param minutes ������
    void SetMinutes(float minutes);

    /// \~chinese
    /// @brief ����Сʱ��
    /// @param hours Сʱ��
    void SetHours(float hours);

    /// \~chinese
    /// @brief ����
    void Sleep() const;

    /// \~chinese
    /// @brief תΪ�ַ���
    String ToString() const;

    /// \~chinese
    /// @brief ����ʱ����ַ���
    /// @param str ʱ����ַ���
    /// @details
    ///   ʱ����ַ����������з��ŵĸ�����, ���Ҵ���ʱ�䵥λ��׺
    ///   ����: "300ms", "-1.5h", "2h45m"
    ///   �����ʱ�䵥λ�� "ms", "s", "m", "h"
    /// @return ��������ʱ���
    /// @throw kiwano::RuntimeError ����һ�����Ϸ��ĸ�ʽʱ�׳�
    static Duration Parse(const String& str);

    static const Duration Ms;      ///< ����
    static const Duration Second;  ///< ��
    static const Duration Minute;  ///< ����
    static const Duration Hour;    ///< Сʱ

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
 * @brief ʱ��
 * @par ʾ����
 * @code
 *   // ��ʱ�����, �ɵõ�һ�� Duration ����
 *   Time t1 = Time::Now();
 *   // �ȴ�һ��ʱ���
 *   Time t2 = Time::Now();
 *   int ms = (t2 - t1).GetMilliseconds();  // ��ȡ��ʱ�����ĺ�����
 * @endcode
 * @note ʱ�����ϵͳʱ���޹أ���˲��ܽ�ʱ���ת��Ϊʱ����
 */
struct KGE_API Time
{
    Time();

    /// \~chinese
    /// @brief �Ƿ�����ʱ
    /// @return ������ʱ������true
    bool IsZero() const;

    /// \~chinese
    /// @brief ��ȡ��ǰʱ��
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
 * @brief ʱ��ʱ��
 */
struct KGE_API ClockTime
{
    ClockTime();

    /// \~chinese
    /// @brief ��ȡ��ǰʱ���
    int64_t GetTimeStamp() const;

    /// \~chinese
    /// @brief ��ȡ�Լ�Ԫ�����ĺ�����
    int64_t GetMillisecondsSinceEpoch() const;

    /// \~chinese
    /// @brief ��ȡ C ����ʱ��
    std::time_t GetCTime() const;

    /// \~chinese
    /// @brief ��ȡ��ǰʱ��
    static ClockTime Now() noexcept;

    /// \~chinese
    /// @brief ʱ���ת��Ϊʱ��
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
