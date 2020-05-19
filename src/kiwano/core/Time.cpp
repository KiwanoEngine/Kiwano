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

#include <kiwano/core/Exception.h>
#include <kiwano/core/Logger.h>
#include <kiwano/core/Time.h>
#include <regex>
#include <unordered_map>
#include <chrono>

namespace kiwano
{
//-------------------------------------------------------
// Time
//-------------------------------------------------------

Time::Time()
    : dur_(0)
{
}

Time::Time(long dur)
    : dur_(dur)
{
}

const Time Time::operator+(const Duration& dur) const
{
    return Time{ dur_ + dur.Milliseconds() };
}

const Time Time::operator-(const Duration& dur) const
{
    return Time{ dur_ - dur.Milliseconds() };
}

Time& Time::operator+=(const Duration& other)
{
    dur_ += other.Milliseconds();
    return (*this);
}

Time& Time::operator-=(const Duration& other)
{
    dur_ -= other.Milliseconds();
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
    return Time{ static_cast<long>(count.QuadPart * millisecs_per_count) };

#else

    using std::chrono::steady_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    const auto      now   = steady_clock::now();
    const long long count = duration_cast<milliseconds>(now.time_since_epoch()).count();
    return Time{ static_cast<long>(count) };

#endif
}

//-------------------------------------------------------
// Duration
//-------------------------------------------------------

const Duration Duration::Ms     = 1L;
const Duration Duration::Second = 1000 * Duration::Ms;
const Duration Duration::Minute = 60 * Duration::Second;
const Duration Duration::Hour   = 60 * Duration::Minute;

namespace
{
const auto duration_regex = std::regex(R"(^[-+]?([0-9]*(\.[0-9]*)?(h|m|s|ms)+)+$)");

typedef std::unordered_map<String, Duration> UnitMap;

const auto unit_map =
    UnitMap{ { "ms", Duration::Ms }, { "s", Duration::Second }, { "m", Duration::Minute }, { "h", Duration::Hour } };

}  // namespace

Duration::Duration()
    : milliseconds_(0)
{
}

Duration::Duration(long milliseconds)
    : milliseconds_(milliseconds)
{
}

float Duration::Seconds() const
{
    long sec = milliseconds_ / Second.milliseconds_;
    long ms  = milliseconds_ % Second.milliseconds_;
    return static_cast<float>(sec) + static_cast<float>(ms) / 1000.f;
}

float Duration::Minutes() const
{
    long min = milliseconds_ / Minute.milliseconds_;
    long ms  = milliseconds_ % Minute.milliseconds_;
    return static_cast<float>(min) + static_cast<float>(ms) / (60 * 1000.f);
}

float Duration::Hours() const
{
    long hour = milliseconds_ / Hour.milliseconds_;
    long ms   = milliseconds_ % Hour.milliseconds_;
    return static_cast<float>(hour) + static_cast<float>(ms) / (60 * 60 * 1000.f);
}

String Duration::ToString() const
{
    if (IsZero())
    {
        return String("0s");
    }

    String result;
    long   total_ms = milliseconds_;
    if (total_ms < 0)
    {
        result.append("-");
        total_ms = -total_ms;
    }

    long hour = total_ms / Hour.milliseconds_;
    long min  = total_ms / Minute.milliseconds_ - hour * 60;
    long sec  = total_ms / Second.milliseconds_ - (hour * 60 * 60 + min * 60);
    long ms   = total_ms % Second.milliseconds_;

    if (hour)
    {
        result.append(std::to_string(hour)).append("h");
        result.append(std::to_string(min)).append("m");
    }
    else if (min)
    {
        result.append(std::to_string(min)).append("m");
    }

    if (ms != 0)
    {
        result.append(std::to_string(static_cast<float>(sec) + static_cast<float>(ms) / 1000.f)).append("s");
    }
    else if (sec != 0)
    {
        result.append(std::to_string(sec)).append("s");
    }
    return result;
}

bool Duration::operator==(const Duration& other) const
{
    return milliseconds_ == other.milliseconds_;
}

bool Duration::operator!=(const Duration& other) const
{
    return milliseconds_ != other.milliseconds_;
}

bool Duration::operator>(const Duration& other) const
{
    return milliseconds_ > other.milliseconds_;
}

bool Duration::operator>=(const Duration& other) const
{
    return milliseconds_ >= other.milliseconds_;
}

bool Duration::operator<(const Duration& other) const
{
    return milliseconds_ < other.milliseconds_;
}

bool Duration::operator<=(const Duration& other) const
{
    return milliseconds_ <= other.milliseconds_;
}

float Duration::operator/(const Duration& other) const
{
    return static_cast<float>(milliseconds_) / other.milliseconds_;
}

const Duration Duration::operator+(const Duration& other) const
{
    return Duration(milliseconds_ + other.milliseconds_);
}

const Duration Duration::operator-(const Duration& other) const
{
    return Duration(milliseconds_ - other.milliseconds_);
}

const Duration Duration::operator-() const
{
    return Duration(-milliseconds_);
}

const Duration Duration::operator*(int val) const
{
    return Duration(milliseconds_ * val);
}

const Duration Duration::operator*(unsigned long long val) const
{
    return Duration(static_cast<long>(milliseconds_ * val));
}

const Duration Duration::operator*(float val) const
{
    return Duration(static_cast<long>(milliseconds_ * val));
}

const Duration Duration::operator*(double val) const
{
    return Duration(static_cast<long>(milliseconds_ * val));
}

const Duration Duration::operator*(long double val) const
{
    return Duration(static_cast<long>(milliseconds_ * val));
}

const Duration Duration::operator/(int val) const
{
    return Duration(milliseconds_ / val);
}

const Duration Duration::operator/(float val) const
{
    return Duration(static_cast<long>(milliseconds_ / val));
}

const Duration Duration::operator/(double val) const
{
    return Duration(static_cast<long>(milliseconds_ / val));
}

Duration& Duration::operator+=(const Duration& other)
{
    milliseconds_ += other.milliseconds_;
    return (*this);
}

Duration& Duration::operator-=(const Duration& other)
{
    milliseconds_ -= other.milliseconds_;
    return (*this);
}

Duration& Duration::operator*=(int val)
{
    milliseconds_ *= val;
    return (*this);
}

Duration& Duration::operator/=(int val)
{
    milliseconds_ = static_cast<long>(milliseconds_ / val);
    return (*this);
}

Duration& Duration::operator*=(float val)
{
    milliseconds_ = static_cast<long>(milliseconds_ * val);
    return (*this);
}

Duration& Duration::operator/=(float val)
{
    milliseconds_ = static_cast<long>(milliseconds_ / val);
    return (*this);
}

Duration& Duration::operator*=(double val)
{
    milliseconds_ = static_cast<long>(milliseconds_ * val);
    return (*this);
}

Duration& Duration::operator/=(double val)
{
    milliseconds_ = static_cast<long>(milliseconds_ / val);
    return (*this);
}

const Duration operator*(int val, const Duration& dur)
{
    return dur * val;
}

const Duration operator/(int val, const Duration& dur)
{
    return dur / val;
}

const Duration operator*(float val, const Duration& dur)
{
    return dur * val;
}

const Duration operator/(float val, const Duration& dur)
{
    return dur / val;
}

const Duration operator*(double val, const Duration& dur)
{
    return dur * val;
}

const Duration operator/(double val, const Duration& dur)
{
    return dur / val;
}

const Duration operator*(long double val, const Duration& dur)
{
    return dur * val;
}

Duration Duration::Parse(const String& format)
{
    bool     negative = false;
    size_t   len      = format.length();
    size_t   pos      = 0;
    Duration ret;

    if (!std::regex_match(format.c_str(), duration_regex))
    {
        KGE_THROW("Duration::Parse failed, invalid duration");
    }

    if (format.empty() || format == "0")
    {
        return ret;
    }

    // 符号位
    if (format[0] == '-' || format[0] == '+')
    {
        negative = (format[0] == '-');
        pos++;
    }

    while (pos < len)
    {
        // 数值
        size_t i = pos;
        for (; i < len; ++i)
        {
            wchar_t ch = format[i];
            if (!(ch == '.' || '0' <= ch && ch <= '9'))
            {
                break;
            }
        }

        String num_str = format.substr(pos, i - pos);

        pos = i;

        if (num_str.empty() || num_str == ".")
            KGE_THROW("Duration::Parse failed, invalid duration");

        // 单位
        for (; i < len; ++i)
        {
            wchar_t ch = format[i];
            if (ch == '.' || '0' <= ch && ch <= '9')
            {
                break;
            }
        }

        String unit_str = format.substr(pos, i - pos);

        pos = i;

        if (unit_map.find(unit_str) == unit_map.end())
            KGE_THROW("Duration::Parse failed, invalid duration");

        double   num  = std::stod(num_str.c_str());
        Duration unit = unit_map.at(unit_str);
        ret += unit * num;
    }

    if (negative)
    {
        ret = -ret;
    }
    return ret;
}

}  // namespace kiwano
