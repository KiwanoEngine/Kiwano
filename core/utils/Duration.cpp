// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "..\e2dutil.h"
#include <regex>


namespace easy2d
{
	const Duration Duration::Millisecond = Duration(1);
	const Duration Duration::Second = 1000 * Duration::Millisecond;
	const Duration Duration::Minute = 60 * Duration::Second;
	const Duration Duration::Hour = 60 * Duration::Minute;

	namespace
	{
		const auto duration_regex = std::wregex(L"[-+]?([0-9]*(\\.[0-9]*)?[a-z]+)+");

		typedef std::map<std::wstring, Duration> UnitMap;
		const auto unit_map = UnitMap{
			{L"ms", Duration::Millisecond},
			{L"s", Duration::Second},
			{L"m", Duration::Minute},
			{L"h", Duration::Hour}
		};
	}

	Duration::Duration()
		: milliseconds_(0)
	{
	}

	Duration::Duration(int milliseconds)
		: milliseconds_(milliseconds)
	{
	}

	int Duration::Milliseconds() const
	{
		return milliseconds_;
	}

	float Duration::Seconds() const
	{
		int64_t sec = milliseconds_ / Second.milliseconds_;
		int64_t ms = milliseconds_ % Second.milliseconds_;
		return static_cast<float>(sec) + static_cast<float>(ms) / 1000.f;
	}

	float Duration::Minutes() const
	{
		int64_t min = milliseconds_ / Minute.milliseconds_;
		int64_t ms = milliseconds_ % Minute.milliseconds_;
		return static_cast<float>(min) + static_cast<float>(ms) / (60 * 1000.f);
	}

	float Duration::Hours() const
	{
		int64_t hour = milliseconds_ / Hour.milliseconds_;
		int64_t ms = milliseconds_ % Hour.milliseconds_;
		return static_cast<float>(hour) + static_cast<float>(ms) / (60 * 60 * 1000.f);
	}

	Duration Duration::Parse(const std::wstring & str)
	{
		size_t len = str.length();
		size_t pos = 0;
		bool negative = false;
		Duration d;

		if (!std::regex_match(str, duration_regex))
		{
			E2D_WARNING("Duration::Parse: invalid duration");
			return d;
		}

		if (str.empty() || str == L"0") { return d; }

		// 符号位
		if (str[0] == L'-' || str[0] == L'+')
		{
			negative = (str[0] == L'-');
			pos++;
		}

		while (pos < len)
		{
			// 数值
			size_t i = pos;
			for (; i < len; ++i)
			{
				wchar_t ch = str[i];
				if (!(ch == L'.' || L'0' <= ch && ch <= L'9'))
				{
					break;
				}
			}

			std::wstring num_str = str.substr(pos, i - pos);
			pos = i;

			if (num_str.empty() || num_str == L".")
			{
				E2D_WARNING("Duration::Parse: invalid duration");
				return Duration();
			}

			// 单位
			for (; i < len; ++i)
			{
				wchar_t ch = str[i];
				if (ch == L'.' || L'0' <= ch && ch <= L'9')
				{
					break;
				}
			}

			std::wstring unit_str = str.substr(pos, i - pos);
			pos = i;

			if (unit_map.find(unit_str) == unit_map.end())
			{
				E2D_WARNING("Duration::Parse: invalid duration");
				return Duration();
			}

			double num = std::stod(num_str);
			Duration unit = unit_map.at(unit_str);
			d += unit * num;
		}

		if (negative)
		{
			d.milliseconds_ = -d.milliseconds_;
		}
		return d;
	}

	bool Duration::operator==(const Duration & other) const
	{
		return milliseconds_ == other.milliseconds_;
	}

	bool Duration::operator!=(const Duration & other) const
	{
		return milliseconds_ != other.milliseconds_;
	}

	bool Duration::operator>(const Duration & other) const
	{
		return milliseconds_ > other.milliseconds_;
	}

	bool Duration::operator>=(const Duration & other) const
	{
		return milliseconds_ >= other.milliseconds_;
	}

	bool Duration::operator<(const Duration & other) const
	{
		return milliseconds_ < other.milliseconds_;
	}

	bool Duration::operator<=(const Duration & other) const
	{
		return milliseconds_ <= other.milliseconds_;
	}

	Duration Duration::operator+(const Duration & other) const
	{
		return Duration(milliseconds_ + other.milliseconds_);
	}

	Duration Duration::operator-(const Duration & other) const
	{
		return Duration(milliseconds_ - other.milliseconds_);
	}

	Duration Duration::operator-() const
	{
		return Duration(-milliseconds_);
	}

	Duration Duration::operator*(int value) const
	{
		return Duration(milliseconds_ * value);
	}

	Duration Duration::operator/(int value) const
	{
		return Duration(milliseconds_ / value);
	}

	Duration Duration::operator*(float value) const
	{
		return Duration(static_cast<int>(milliseconds_ * value));
	}

	Duration Duration::operator/(float value) const
	{
		return Duration(static_cast<int>(milliseconds_ / value));
	}

	Duration Duration::operator*(double value) const
	{
		return Duration(static_cast<int>(milliseconds_ * value));
	}

	Duration Duration::operator/(double value) const
	{
		return Duration(static_cast<int>(milliseconds_ / value));
	}

	Duration & Duration::operator+=(const Duration &other)
	{
		milliseconds_ += other.milliseconds_;
		return (*this);
	}

	Duration & Duration::operator-=(const Duration &other)
	{
		milliseconds_ -= other.milliseconds_;
		return (*this);
	}

	Duration & Duration::operator*=(int value)
	{
		milliseconds_ *= value;
		return (*this);
	}

	Duration & Duration::operator/=(int value)
	{
		milliseconds_ /= value;
		return (*this);
	}

	Duration & Duration::operator*=(float value)
	{
		milliseconds_ = static_cast<int>(milliseconds_ * value);
		return (*this);
	}

	Duration & Duration::operator/=(float value)
	{
		milliseconds_ = static_cast<int>(milliseconds_ / value);
		return (*this);
	}

	Duration & Duration::operator*=(double value)
	{
		milliseconds_ = static_cast<int>(milliseconds_ * value);
		return (*this);
	}

	Duration & Duration::operator/=(double value)
	{
		milliseconds_ = static_cast<int>(milliseconds_ / value);
		return (*this);
	}

	Duration operator*(int value, const Duration & dur)
	{
		return dur * value;
	}

	Duration operator/(int value, const Duration & dur)
	{
		return dur / value;
	}

	Duration operator*(float value, const Duration & dur)
	{
		return dur * value;
	}

	Duration operator/(float value, const Duration & dur)
	{
		return dur / value;
	}

	Duration operator*(double value, const Duration & dur)
	{
		return dur * value;
	}

	Duration operator/(double value, const Duration & dur)
	{
		return dur / value;
	}

}
