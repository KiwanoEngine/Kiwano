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

namespace easy2d
{
	namespace time
	{
		// ʱ���
		//
		// ʱ��α�ʾ��:
		//     5 ��: time::Second * 5
		//     1.5 Сʱ: time::Hour * 1.5
		//     3 Сʱ 45 �� 15 ��: time::Hour * 3 + time::Minute * 45 + time::Second * 15
		// �� VS2015 �����߰汾����ʹ�� time literals:
		//     5 ��: 5_s
		//     1.5 Сʱ: 1.5_h
		//     3 Сʱ 45 �� 15 ��: 3_h + 45_m + 15_s
		//
		struct Duration
		{
			Duration();

			Duration(
				long milliseconds
			);

			// ת��Ϊ����
			inline long Milliseconds() const { return milliseconds_; }

			// ת��Ϊ��
			float Seconds() const;

			// ת��Ϊ����
			float Minutes() const;

			// ת��ΪСʱ
			float Hours() const;

			// ʱ���Ƿ�����
			inline bool IsZero() const { return milliseconds_ == 0LL; }

			// תΪ�ַ���
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

			friend std::wostream& operator<< (std::wostream &, const Duration &);
			friend std::wistream& operator>> (std::wistream &, Duration &);

		private:
			long milliseconds_;
		};

		/* Ԥ�����ʱ��� */
		extern const Duration Millisecond;	// ����
		extern const Duration Second;		// ��
		extern const Duration Minute;		// ����
		extern const Duration Hour;			// Сʱ


		// ʱ��
		//
		// ��ȡ��ǰʱ��: TimePoint now = time::Now();
		// ��ʱ�����, �õ�һ�� Duration ����, ����:
		//     TimePoint t1, t2;
		//     int ms = (t2 - t1).Milliseconds();  // ��ȡ��ʱ�����ĺ�����
		// 
		struct TimePoint
		{
			TimePoint();

			TimePoint(long);

			// �Ƿ�����ʱ
			inline bool IsZero() const { return dur == 0; }

			const TimePoint operator + (const Duration &) const;
			const TimePoint operator - (const Duration &) const;

			TimePoint& operator += (const Duration &);
			TimePoint& operator -= (const Duration &);

			const Duration operator - (const TimePoint &) const;

		private:
			long dur;
		};

		// ��ȡ��ǰʱ��
		// 
		// ���ڸ�ʱ������ϵͳ����ʱ�俪ʼ����, �����޷���ʽ����ʱ��, 
		// Ҳ�޷���ø�ʱ��� Unix ʱ���
		TimePoint Now() E2D_NOEXCEPT;

		// ʱ��θ�ʽ��
		//
		// ʱ����ַ����������з��ŵĸ�����, ���Ҵ���ʱ�䵥λ��׺
		// ����: "300ms", "-1.5h", "2h45m"
		// �����ʱ�䵥λ�� "ms", "s", "m", "h"
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
