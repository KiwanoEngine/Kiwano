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

#include <chrono>  // std::chrono
#include <kiwano/core/Time.h>

namespace kiwano
{
//-------------------------------------------------------
// Time
//-------------------------------------------------------

Time::Time()
    : dur_(0)
{
}

Time::Time(int64_t dur)
    : dur_(dur)
{
}

const Time Time::operator+(const Duration& dur) const
{
    return Time{ dur_ + dur.GetMilliseconds() };
}

const Time Time::operator-(const Duration& dur) const
{
    return Time{ dur_ - dur.GetMilliseconds() };
}

Time& Time::operator+=(const Duration& other)
{
    dur_ += other.GetMilliseconds();
    return (*this);
}

Time& Time::operator-=(const Duration& other)
{
    dur_ -= other.GetMilliseconds();
    return (*this);
}

const Duration Time::operator-(const Time& other) const
{
    return Duration(dur_ - other.dur_);
}

Time Time::Now() noexcept
{
#if defined(KGE_PLATFORM_WINDOWS)

    static double millisecs_per_count = {};
    if (millisecs_per_count == 0)
    {
        LARGE_INTEGER freq = {};
        // the Function will always succceed on systems that run Windows XP or later
        QueryPerformanceFrequency(&freq);
        millisecs_per_count = 1000.0 / static_cast<double>(freq.QuadPart);
    }

    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return Time{ static_cast<int64_t>(count.QuadPart * millisecs_per_count) };

#else

    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    const auto now   = steady_clock::now();
    const auto count = duration_cast<milliseconds>(now.time_since_epoch()).count();
    return Time{ static_cast<int64_t>(count) };

#endif
}

//-------------------------------------------------------
// ClockTime
//-------------------------------------------------------

ClockTime::ClockTime()
    : ms_since_epoch_(0)
{
}

int64_t ClockTime::GetTimeStamp() const
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;

    const auto timestamp = duration_cast<seconds>(milliseconds(ms_since_epoch_)).count();
    return static_cast<int64_t>(timestamp);
}

int64_t ClockTime::GetMillisecondsSinceEpoch() const
{
    return ms_since_epoch_;
}

std::time_t ClockTime::GetCTime() const
{
    return static_cast<time_t>(GetTimeStamp());
}

ClockTime::ClockTime(int64_t ms_since_epoch)
    : ms_since_epoch_(ms_since_epoch)
{
}

ClockTime ClockTime::FromTimeStamp(int64_t timestamp) noexcept
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;

    const auto ms = duration_cast<milliseconds>(seconds(timestamp)).count();
    return ClockTime(static_cast<int64_t>(ms));
}

ClockTime ClockTime::Now() noexcept
{
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::system_clock;

    const auto now   = system_clock::now();
    const auto count = duration_cast<milliseconds>(now.time_since_epoch()).count();
    return ClockTime{ static_cast<int64_t>(count) };
}

const Duration ClockTime::operator-(const ClockTime& other) const
{
    return Duration(ms_since_epoch_ - other.ms_since_epoch_);
}

const ClockTime ClockTime::operator+(const Duration& dur) const
{
    return ClockTime{ ms_since_epoch_ + dur.GetMilliseconds() };
}

const ClockTime ClockTime::operator-(const Duration& dur) const
{
    return ClockTime{ ms_since_epoch_ - dur.GetMilliseconds() };
}

ClockTime& ClockTime::operator+=(const Duration& other)
{
    ms_since_epoch_ += other.GetMilliseconds();
    return (*this);
}

ClockTime& ClockTime::operator-=(const Duration& other)
{
    ms_since_epoch_ -= other.GetMilliseconds();
    return (*this);
}

}  // namespace kiwano
