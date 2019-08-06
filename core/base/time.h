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
#include <chrono>

namespace easy2d
{
	namespace time
	{
		// ʱ���
		//
		// Usage:
		//     ʱ��α�ʾ��:
		//         5 ��: time::Second * 5
		//         1.5 Сʱ: time::Hour * 1.5
		//         3 Сʱ 45 �� 15 ��: time::Hour * 3 + time::Minute * 45 + time::Second * 15
		//     ʱ��θ�ʽ��: auto d = time::ParseDuration(L"1h35m");  // 1Сʱ35����
		// 
		class Duration
		{
		public:
			Duration();

			explicit Duration(
				int milliseconds
			);

			// ת��Ϊ����
			int Milliseconds() const;

			// ת��Ϊ��
			float Seconds() const;

			// ת��Ϊ����
			float Minutes() const;

			// ת��ΪСʱ
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

		extern const Duration Millisecond;	// ����
		extern const Duration Second;		// ��
		extern const Duration Minute;		// ����
		extern const Duration Hour;			// Сʱ


		// ʱ��
		//
		// Usage:
		//     ��ȡ��ǰʱ��: Time now = time::Now();
		//     ʱ�����:
		//         ��ʱ�����, �õ�һ�� Duration ����, ����:
		//         Time t1 = time::Now();
		//         ...  // ��Щʲô
		//         Time t2 = time::Now();
		//         auto duration = t2 - t1;
		//         ��ȡ��ʱ�����ĺ�����:
		//         int ms = duration.Milliseconds();
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

			// ��ȡʱ���
			time_t GetTimeStamp() const;

			// �Ƿ�����ʱ
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

		// ��ȡ��ǰʱ��
		TimePoint Now();

		// ʱ��θ�ʽ��
		// ʱ����ַ����������з��ŵĸ�����, ���Ҵ���ʱ�䵥λ��׺
		// ����: "300ms", "-1.5h", "2h45m"
		// �����ʱ�䵥λ�� "ms", "s", "m", "h"
		Duration ParseDuration(const std::wstring& parse_str);
	}
}