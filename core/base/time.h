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

#pragma once
#include "macros.h"
#include <cstdint>

namespace easy2d
{
	namespace time
	{
		// 时间段
		//
		// Usage:
		//     时间段表示法:
		//         5 秒: time::Second * 5
		//         1.5 小时: time::Hour * 1.5
		//         3 小时 45 分 15 秒: time::Hour * 3 + time::Minute * 45 + time::Second * 15
		//     时间段格式化: auto d = time::ParseDuration(L"1h35m");  // 1小时35分钟
		//     在 VS2015 及更高版本可以使用 time literals:
		//         5 秒: 5_s
		//         1.5 小时: 1.5_h
		//         3 小时 45 分 15 秒: 3_h + 45_m + 15_s
		//
		class Duration
		{
		public:
			Duration();

			explicit Duration(
				int64_t milliseconds
			);

			// 转化为毫秒
			inline int64_t Milliseconds() const { return milliseconds_; }

			// 转化为秒
			float Seconds() const;

			// 转化为分钟
			float Minutes() const;

			// 转化为小时
			float Hours() const;

			// 时长是否是零
			inline bool IsZero() const { return milliseconds_ == 0LL; }

			// 转为字符串
			std::wstring ToString() const;

			bool operator== (const Duration &) const;
			bool operator!= (const Duration &) const;
			bool operator> (const Duration &) const;
			bool operator>= (const Duration &) const;
			bool operator< (const Duration &) const;
			bool operator<= (const Duration &) const;

			float operator / (const Duration &) const;

			const Duration operator + (const Duration &) const;
			const Duration operator - (const Duration &) const;
			const Duration operator - () const;
			const Duration operator * (int) const;
			const Duration operator * (unsigned long long) const;
			const Duration operator * (float) const;
			const Duration operator * (double) const;
			const Duration operator * (long double) const;
			const Duration operator / (int) const;
			const Duration operator / (unsigned long long) const;
			const Duration operator / (float) const;
			const Duration operator / (double) const;
			const Duration operator / (long double) const;

			Duration& operator += (const Duration &);
			Duration& operator -= (const Duration &);
			Duration& operator *= (int);
			Duration& operator *= (unsigned long long);
			Duration& operator *= (float);
			Duration& operator *= (double);
			Duration& operator *= (long double);
			Duration& operator /= (int);
			Duration& operator /= (unsigned long long);
			Duration& operator /= (float);
			Duration& operator /= (double);
			Duration& operator /= (long double);

			friend const Duration operator* (int, const Duration &);
			friend const Duration operator* (unsigned long long, const Duration &);
			friend const Duration operator* (float, const Duration &);
			friend const Duration operator* (double, const Duration &);
			friend const Duration operator* (long double, const Duration &);
			friend const Duration operator/ (int, const Duration &);
			friend const Duration operator/ (unsigned long long, const Duration &);
			friend const Duration operator/ (float, const Duration &);
			friend const Duration operator/ (double, const Duration &);
			friend const Duration operator/ (long double, const Duration &);

			friend std::wostream& operator<< (std::wostream &, const Duration &);
			friend std::wistream& operator>> (std::wistream &, Duration &);

		private:
			int64_t milliseconds_;
		};

		extern const Duration Millisecond;	// 毫秒
		extern const Duration Second;		// 秒
		extern const Duration Minute;		// 分钟
		extern const Duration Hour;			// 小时


		// 时间
		//
		// Usage:
		//     获取当前时间: Time now = time::Now();
		//     时间操作:
		//         两时间相减, 得到一个 Duration 对象, 例如:
		//         Time t1 = time::Now();
		//         ...  // 做些什么
		//         Time t2 = time::Now();
		//         auto duration = t2 - t1;
		//         获取两时间相差的毫秒数:
		//         int ms = duration.Milliseconds();
		//     注: 由于该时间点基于系统启动时间开始计算, 所以无法格式化该时间,
		//         也无法获得该时间的 Unix 时间戳
		// 
		class TimePoint
		{
		public:
			TimePoint();

			explicit TimePoint(
				const Duration&
			);

			TimePoint(
				const TimePoint& other
			);

			TimePoint(
				TimePoint&& other
			);

			// 是否是零时
			inline bool IsZero() const { return dur.IsZero(); }

			const TimePoint operator + (const Duration &) const;
			const TimePoint operator - (const Duration &) const;

			TimePoint& operator += (const Duration &);
			TimePoint& operator -= (const Duration &);

			const Duration operator - (const TimePoint &) const;

			TimePoint& operator = (const TimePoint &) E2D_NOEXCEPT;
			TimePoint& operator = (TimePoint &&) E2D_NOEXCEPT;

		private:
			Duration dur;
		};

		// 获取当前时间
		TimePoint Now() E2D_NOEXCEPT;

		// 时间段格式化
		// 时间段字符串允许是有符号的浮点数, 并且带有时间单位后缀
		// 例如: "300ms", "-1.5h", "2h45m"
		// 允许的时间单位有 "ms", "s", "m", "h"
		Duration ParseDuration(const std::wstring& parse_str);
	}
}

namespace easy2d
{
	using namespace time;
}

#if VS_VER >= VS_2015

namespace easy2d
{
	inline namespace literals
	{
		inline const easy2d::time::Duration operator "" _ms(long double val)
		{
			return easy2d::time::Millisecond * val;
		}

		inline const easy2d::time::Duration operator "" _s(long double val)
		{
			return easy2d::time::Second * val;
		}

		inline const easy2d::time::Duration operator "" _m(long double val)
		{
			return easy2d::time::Minute * val;
		}

		inline const easy2d::time::Duration operator "" _h(long double val)
		{
			return easy2d::time::Hour * val;
		}

		inline const easy2d::time::Duration operator "" _ms(unsigned long long val)
		{
			return easy2d::time::Millisecond * val;
		}

		inline const easy2d::time::Duration operator "" _s(unsigned long long val)
		{
			return easy2d::time::Second * val;
		}

		inline const easy2d::time::Duration operator "" _m(unsigned long long val)
		{
			return easy2d::time::Minute * val;
		}

		inline const easy2d::time::Duration operator "" _h(unsigned long long val)
		{
			return easy2d::time::Hour * val;
		}
	}

	namespace time
	{
		using namespace easy2d::literals;
	}
}

#endif
