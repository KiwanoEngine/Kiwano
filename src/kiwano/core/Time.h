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
    /// @brief �Ƿ�����ʱ
    /// @return ������ʱ������true
    bool IsZero() const;

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

inline bool Time::IsZero() const
{
    return dur_ == 0;
}

inline bool ClockTime::IsZero() const
{
    return ms_since_epoch_ == 0;
}

}  // namespace kiwano
