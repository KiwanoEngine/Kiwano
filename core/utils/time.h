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
#include "..\e2dmacros.h"

namespace easy2d
{
	namespace time
	{
		// 时间段
		//
		// Usage:
		//     获取当前时间: Time now = Time::Now();
		//     时间操作:
		//         两时间相减, 得到一个 Duration 对象, 例如:
		//         Time t1 = Time::Now();
		//         ...  // 做些什么
		//         Time t2 = Time::Now();
		//         auto duration = t2 - t1;
		//         获取两时间相差的毫秒数:
		//         int ms = duration.Milliseconds();
		//     时间段表示法:
		//         5 秒: Time::Second * 5
		//         1.5 小时: Time::Hour * 1.5
		//         3 小时 45 分 15 秒: Time::Hour * 3 + Time::Minute * 45 + Time::Second * 15
		//     时间段格式化: auto d = Time::ParseDuration(L"1h35m");  // 1小时35分钟
		// 
		class Duration
		{
		public:
			Duration();

			explicit Duration(
				int milliseconds
			);

			// 转化为毫秒
			int Milliseconds() const;

			// 转化为秒
			float Seconds() const;

			// 转化为分钟
			float Minutes() const;

			// 转化为小时
			float Hours() const;

			bool operator== (const Duration &) const;
			bool operator!= (const Duration &) const;
			bool operator> (const Duration &) const;
			bool operator>= (const Duration &) const;
			bool operator< (const Duration &) const;
			bool operator<= (const Duration &) const;

			Duration operator + (const Duration &) const;
			Duration operator - (const Duration &) const;
			Duration operator - () const;
			Duration operator * (int) const;
			Duration operator * (float) const;
			Duration operator * (double) const;
			Duration operator / (int) const;
			Duration operator / (float) const;
			Duration operator / (double) const;

			Duration& operator += (const Duration &);
			Duration& operator -= (const Duration &);
			Duration& operator *= (int);
			Duration& operator *= (float);
			Duration& operator *= (double);
			Duration& operator /= (int);
			Duration& operator /= (float);
			Duration& operator /= (double);

			friend Duration operator* (int, const Duration &);
			friend Duration operator* (float, const Duration &);
			friend Duration operator* (double, const Duration &);
			friend Duration operator/ (int, const Duration &);
			friend Duration operator/ (float, const Duration &);
			friend Duration operator/ (double, const Duration &);

		private:
			int milliseconds_;
		};

		extern const Duration Millisecond;	// 毫秒
		extern const Duration Second;		// 秒
		extern const Duration Minute;		// 分钟
		extern const Duration Hour;			// 小时


		// 时间
		//
		// Usage:
		//     获取当前时间: Time now = Time::Now();
		//     时间操作:
		//         两时间相减, 得到一个 Duration 对象, 例如:
		//         Time t1 = Time::Now();
		//         ...  // 做些什么
		//         Time t2 = Time::Now();
		//         auto duration = t2 - t1;
		//         获取两时间相差的毫秒数:
		//         int ms = duration.Milliseconds();
		//     时间段表示法:
		//         5 秒: Time::Second * 5
		//         1.5 小时: Time::Hour * 1.5
		//         3 小时 45 分 15 秒: Time::Hour * 3 + Time::Minute * 45 + Time::Second * 15
		//     时间段格式化: auto d = Time::ParseDuration(L"1h35m");  // 1小时35分钟
		// 
		class TimePoint
		{
		public:
			TimePoint();

			explicit TimePoint(
				std::chrono::steady_clock::time_point
			);

			TimePoint(
				const TimePoint& other
			);

			TimePoint(
				TimePoint&& other
			);

			// 获取时间戳
			time_t GetTimeStamp() const;

			// 是否是零时
			bool IsZero() const;

			TimePoint operator + (const Duration &) const;
			TimePoint operator - (const Duration &) const;

			TimePoint& operator += (const Duration &);
			TimePoint& operator -= (const Duration &);

			Duration operator - (const TimePoint &) const;

			TimePoint& operator = (const TimePoint &) E2D_NOEXCEPT;
			TimePoint& operator = (TimePoint &&) E2D_NOEXCEPT;

		private:
			std::chrono::steady_clock::time_point time_;
		};

		// 获取当前时间
		extern TimePoint Now();

		// 时间段格式化
		// 时间段字符串允许是有符号的浮点数, 并且带有时间单位后缀
		// 例如: "300ms", "-1.5h", "2h45m"
		// 允许的时间单位有 "ms", "s", "m", "h"
		extern Duration ParseDuration(const std::wstring& parse_str);
	}
}