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
#include <ostream>
#include <istream>

#include "../macros.h"
#include "../core/core.h"

namespace kiwano
{
	namespace time
	{
		// 时间段
		//
		// 时间段表示法:
		//     5 秒: time::Sec * 5
		//     1.5 小时: time::Hour * 1.5
		//     3 小时 45 分 15 秒: time::Hour * 3 + time::Min * 45 + time::Sec * 15
		// 在 VS2015 及更高版本可以使用 time literals:
		//     5 秒: 5_s
		//     1.5 小时: 1.5_h
		//     3 小时 45 分 15 秒: 3_h + 45_m + 15_s
		//
		struct KGE_API Duration
		{
			Duration();

			Duration(
				long milliseconds
			);

			// 转化为毫秒
			inline long Milliseconds() const		{ return milliseconds_; }

			// 转化为秒
			float Seconds() const;

			// 转化为分钟
			float Minutes() const;

			// 转化为小时
			float Hours() const;

			// 时长是否是零
			inline bool IsZero() const				{ return milliseconds_ == 0LL; }

			inline void SetMilliseconds(long ms)	{ milliseconds_ = ms; }

			inline void SetSeconds(float seconds)	{ milliseconds_ = static_cast<long>(seconds * 1000.f); }

			inline void SetMinutes(float minutes)	{ milliseconds_ = static_cast<long>(minutes * 60 * 1000.f); }

			inline void SetHours(float hours)		{ milliseconds_ = static_cast<long>(hours * 60 * 60 * 1000.f); }

			// 转为字符串
			String ToString() const;

			inline operator bool() const			{ return !IsZero(); }

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
			const Duration operator / (float) const;
			const Duration operator / (double) const;

			Duration& operator += (const Duration &);
			Duration& operator -= (const Duration &);
			Duration& operator *= (int);
			Duration& operator *= (float);
			Duration& operator *= (double);
			Duration& operator /= (int);
			Duration& operator /= (float);
			Duration& operator /= (double);

			friend const Duration operator* (int, const Duration &);
			friend const Duration operator* (float, const Duration &);
			friend const Duration operator* (double, const Duration &);
			friend const Duration operator* (long double, const Duration &);
			friend const Duration operator/ (int, const Duration &);
			friend const Duration operator/ (float, const Duration &);
			friend const Duration operator/ (double, const Duration &);

		public:
			// 时间段格式化
			//
			// 时间段字符串允许是有符号的浮点数, 并且带有时间单位后缀
			// 例如: "300ms", "-1.5h", "2h45m"
			// 允许的时间单位有 "ms", "s", "m", "h"
			static Duration Parse(const String& parse_str);

			template <typename _Char>
			friend inline std::basic_ostream<_Char>& operator<<(std::basic_ostream<_Char>& out, const Duration& dur)
			{
				return out << dur.ToString();
			}

			template <typename _Char>
			friend inline std::basic_istream<_Char>& operator>>(std::basic_istream<_Char>& in, Duration& dur)
			{
				String str;
				if (in >> str)
				{
					dur = Duration::Parse(str);
				}
				return in;
			}

		private:
			long milliseconds_;
		};

		/* 预定义的时间段 */
		KGE_API extern const Duration Ms;		// 毫秒
		KGE_API extern const Duration Sec;		// 秒
		KGE_API extern const Duration Min;		// 分钟
		KGE_API extern const Duration Hour;		// 小时


		// 时间
		//
		// 获取当前时间: Time now = Time::Now();
		// 两时间相减, 得到一个 Duration 对象, 例如:
		//     Time t1, t2;
		//     int ms = (t2 - t1).Milliseconds();  // 获取两时间相差的毫秒数
		// 
		struct KGE_API Time
		{
			Time();

			Time(long);

			// 是否是零时
			inline bool IsZero() const { return dur_ == 0; }

			const Time operator + (const Duration &) const;
			const Time operator - (const Duration &) const;

			Time& operator += (const Duration &);
			Time& operator -= (const Duration &);

			const Duration operator - (const Time &) const;

		public:
			// 获取当前时间
			// 由于该时间点基于系统启动时间开始计算, 所以无法格式化该时间, 
			// 也无法获得该时间的 Unix 时间戳
			static Time Now() noexcept;

		private:
			long dur_;
		};
	}
}

namespace kiwano
{
	using namespace time;
}

#if KGE_VS_VER > KGE_VS_2013

namespace kiwano
{
	inline namespace literals
	{
		inline const kiwano::time::Duration operator "" _ms(long double val)
		{
			return kiwano::time::Ms * val;
		}

		inline const kiwano::time::Duration operator "" _s(long double val)
		{
			return kiwano::time::Sec * val;
		}

		inline const kiwano::time::Duration operator "" _m(long double val)
		{
			return kiwano::time::Min * val;
		}

		inline const kiwano::time::Duration operator "" _h(long double val)
		{
			return kiwano::time::Hour * val;
		}

		inline const kiwano::time::Duration operator "" _ms(unsigned long long val)
		{
			return kiwano::time::Ms * val;
		}

		inline const kiwano::time::Duration operator "" _s(unsigned long long val)
		{
			return kiwano::time::Sec * val;
		}

		inline const kiwano::time::Duration operator "" _m(unsigned long long val)
		{
			return kiwano::time::Min * val;
		}

		inline const kiwano::time::Duration operator "" _h(unsigned long long val)
		{
			return kiwano::time::Hour * val;
		}
	}

	namespace time
	{
		using namespace kiwano::literals;
	}
}

#endif
