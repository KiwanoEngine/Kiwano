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
#include <kiwano/macros.h>
#include <kiwano/common/common.h>

namespace kiwano
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
	struct KGE_API Duration
	{
		Duration();

		Duration(
			long milliseconds
		);

		// ת��Ϊ����
		long Milliseconds() const;

		// ת��Ϊ��
		float Seconds() const;

		// ת��Ϊ����
		float Minutes() const;

		// ת��ΪСʱ
		float Hours() const;

		// ʱ���Ƿ�����
		bool IsZero() const;

		void SetMilliseconds(long ms);

		void SetSeconds(float seconds);

		void SetMinutes(float minutes);

		void SetHours(float hours);

		// תΪ�ַ���
		String ToString() const;

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

		// ʱ��θ�ʽ��
		//
		// ʱ����ַ����������з��ŵĸ�����, ���Ҵ���ʱ�䵥λ��׺
		// ����: "300ms", "-1.5h", "2h45m"
		// �����ʱ�䵥λ�� "ms", "s", "m", "h"
		static Duration Parse(const String& parse_str);

		static const Duration Ms;		// ����
		static const Duration Second;		// ��
		static const Duration Minute;		// ����
		static const Duration Hour;		// Сʱ

	private:
		long milliseconds_;
	};


	// ʱ��
	//
	// ��ȡ��ǰʱ��: Time now = Time::Now();
	// ��ʱ�����, �õ�һ�� Duration ����, ����:
	//     Time t1, t2;
	//     int ms = (t2 - t1).Milliseconds();  // ��ȡ��ʱ�����ĺ�����
	// 
	struct KGE_API Time
	{
		Time();

		// �Ƿ�����ʱ
		bool IsZero() const;

		const Duration operator -(const Time&) const;

		const Time operator +(const Duration &) const;
		const Time operator -(const Duration &) const;

		Time& operator +=(const Duration &);
		Time& operator -=(const Duration &);

		// ��ȡ��ǰʱ��
		// ���ڸ�ʱ������ϵͳ����ʱ�俪ʼ����, �����޷���ʽ����ʱ��, 
		// Ҳ�޷���ø�ʱ��� Unix ʱ���
		static Time Now() noexcept;

	private:
		Time(long ms);

	private:
		long dur_;
	};

	
	inline long Duration::Milliseconds() const		{ return milliseconds_; }

	inline bool Duration::IsZero() const			{ return milliseconds_ == 0LL; }

	inline void Duration::SetMilliseconds(long ms)	{ milliseconds_ = ms; }

	inline void Duration::SetSeconds(float seconds)	{ milliseconds_ = static_cast<long>(seconds * 1000.f); }

	inline void Duration::SetMinutes(float minutes)	{ milliseconds_ = static_cast<long>(minutes * 60 * 1000.f); }

	inline void Duration::SetHours(float hours)		{ milliseconds_ = static_cast<long>(hours * 60 * 60 * 1000.f); }

	inline bool Time::IsZero() const				{ return dur_ == 0; }
}

#if KGE_VS_VER > KGE_VS_2013

namespace kiwano
{
	inline namespace literals
	{
		inline const kiwano::Duration operator "" _ms(long double val)
		{
			return kiwano::Duration::Ms * val;
		}

		inline const kiwano::Duration operator "" _s(long double val)
		{
			return kiwano::Duration::Second * val;
		}

		inline const kiwano::Duration operator "" _m(long double val)
		{
			return kiwano::Duration::Minute * val;
		}

		inline const kiwano::Duration operator "" _h(long double val)
		{
			return kiwano::Duration::Hour * val;
		}

		inline const kiwano::Duration operator "" _ms(unsigned long long val)
		{
			return kiwano::Duration::Ms * val;
		}

		inline const kiwano::Duration operator "" _s(unsigned long long val)
		{
			return kiwano::Duration::Second * val;
		}

		inline const kiwano::Duration operator "" _m(unsigned long long val)
		{
			return kiwano::Duration::Minute * val;
		}

		inline const kiwano::Duration operator "" _h(unsigned long long val)
		{
			return kiwano::Duration::Hour * val;
		}
	}
}

#endif
