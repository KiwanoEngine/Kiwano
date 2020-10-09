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

#include <regex>  // std::regex
#include <unordered_map>  // std::unordered_map
#include <chrono>  // std::chrono::milliseconds
#include <thread>  // std::this_thread::sleep_for
#include <kiwano/core/Duration.h>
#include <kiwano/utils/Logger.h>  // KGE_THROW

namespace kiwano
{

const Duration time::Millisecond = 1L;
const Duration time::Second      = 1000 * time::Millisecond;
const Duration time::Minute      = 60 * time::Second;
const Duration time::Hour        = 60 * time::Minute;

namespace
{
const auto duration_regex = std::regex(R"(^[-+]?([0-9]*(\.[0-9]*)?(h|m|s|ms)+)+$)");

typedef std::unordered_map<String, Duration> UnitMap;

const auto unit_map = UnitMap{ { "ms", time::Millisecond }, { "s", time::Second }, { "m", time::Minute }, { "h", time::Hour } };
}  // namespace

float Duration::GetSeconds() const
{
    auto sec = milliseconds_ / time::Second.milliseconds_;
    auto ms  = milliseconds_ % time::Second.milliseconds_;
    return static_cast<float>(sec + ms) / 1000.f;
}

float Duration::GetMinutes() const
{
    auto min = milliseconds_ / time::Minute.milliseconds_;
    auto ms  = milliseconds_ % time::Minute.milliseconds_;
    return static_cast<float>(min + ms) / (60 * 1000.f);
}

float Duration::GetHours() const
{
    auto hour = milliseconds_ / time::Hour.milliseconds_;
    auto ms   = milliseconds_ % time::Hour.milliseconds_;
    return static_cast<float>(hour + ms) / (60 * 60 * 1000.f);
}

void Duration::Sleep() const
{
    using std::chrono::milliseconds;
    using std::this_thread::sleep_for;

    if (milliseconds_)
    {
        sleep_for(milliseconds(milliseconds_));
    }
}

String Duration::ToString() const
{
    if (IsZero())
    {
        return String("0s");
    }

    StringStream stream;

    int64_t total_ms = milliseconds_;
    if (total_ms < 0)
    {
        stream << "-";
        total_ms = -total_ms;
    }

    int64_t hour = total_ms / time::Hour.milliseconds_;
    int64_t min  = total_ms / time::Minute.milliseconds_ - hour * 60;
    int64_t sec  = total_ms / time::Second.milliseconds_ - (hour * 60 * 60 + min * 60);
    int64_t ms   = total_ms % time::Second.milliseconds_;

    if (hour)
    {
        stream << hour << 'h' << min << 'm';
    }
    else if (min)
    {
        stream << min << 'm';
    }

    if (ms != 0)
    {
        stream << float(sec) + float(ms) / 1000.f << 's';
    }
    else if (sec != 0)
    {
        stream << sec << 's';
    }
    return stream.str();
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
