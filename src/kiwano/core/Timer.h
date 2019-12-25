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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/time.h>

namespace kiwano
{
	class TimerManager;

	KGE_DECLARE_SMART_PTR(Timer);

	/// \~chinese
    /// @brief ��ʱ��
	/// @details ��ʱ������ÿ��һ��ʱ��ִ��һ�λص��������ҿ���ָ��ִ���ܴ���
	class KGE_API Timer
		: public ObjectBase
		, protected IntrusiveListItem<TimerPtr>
	{
		friend class TimerManager;
		friend IntrusiveList<TimerPtr>;

	public:
		/// \~chinese
		/// @brief ��ʱ���ص�����
		/// @details �ص�������һ�������Ƕ�ʱ�������ڶ��������Ǿ����ϴθ��µ�ʱ����
		using Callback = Function<void(Timer* /* self */, Duration /* dt */)>;

		/// \~chinese
		/// @brief ����ն�ʱ��
		Timer();

		/// \~chinese
		/// @brief ���춨ʱ��
		/// @param cb �ص�����
		/// @param interval ʱ����
		/// @param times ִ�д������� -1 Ϊ����ִ�У�
		Timer(Callback const& cb, Duration interval, int times = -1);

		/// \~chinese
		/// @brief ���춨ʱ��
		/// @param name ����
		/// @param cb �ص�����
		/// @param interval ʱ����
		/// @param times ִ�д������� -1 Ϊ����ִ�У�
		Timer(String const& name, Callback const& cb, Duration interval, int times = -1);

		/// \~chinese
		/// @brief ������ʱ��
		void Start();

		/// \~chinese
		/// @brief ֹͣ��ʱ��
		void Stop();

		/// \~chinese
		/// @brief �Ƴ���ʱ��
		void Remove();

		/// \~chinese
		/// @brief ��ʱ���Ƿ�������
		bool IsRunning() const;

		/// \~chinese
		/// @brief ��ʱ���Ƿ���Ƴ�
		bool IsRemoveable() const;

		/// \~chinese
		/// @brief ��ȡ��ʱ��ִ�й��ص������Ĵ���
		int GetRunTimes() const;

		/// \~chinese
		/// @brief ��ȡ��ʱ��ִ�лص��������ܴ���
		int GetTotalRunTimes() const;

		/// \~chinese
		/// @brief ���ö�ʱ��ִ�лص��������ܴ���
		void SetTotalRunTimes(int times);

		/// \~chinese
		/// @brief ��ȡ��ʱ��ִ��ʱ����
		Duration GetInterval() const;

		/// \~chinese
		/// @brief ���ö�ʱ��ִ��ʱ����
		void SetInterval(Duration interval);

		/// \~chinese
		/// @brief ��ȡ��ʱ���ص�����
		Callback GetCallback() const;

		/// \~chinese
		/// @brief ���ö�ʱ���ص�����
		void SetCallback(const Callback& callback);

	private:
		/// \~chinese
		/// @brief ���¶�ʱ��
		void Update(Duration dt);

		/// \~chinese
		/// @brief ���ö�ʱ��
		void Reset();

	private:
		bool		running_;
		bool		removeable_;
		int			run_times_;
		int			total_times_;
		Duration	interval_;
		Duration	elapsed_;
		Callback	callback_;
	};

	inline void Timer::Start()
	{
		running_ = true;
	}

	inline void Timer::Stop()
	{
		running_ = false;
	}

	inline void Timer::Remove()
	{
		removeable_ = true;
	}

	inline bool Timer::IsRunning() const
	{
		return running_;
	}

	inline bool Timer::IsRemoveable() const
	{
		return removeable_;
	}

	inline int Timer::GetRunTimes() const
	{
		return run_times_;
	}

	inline int Timer::GetTotalRunTimes() const
	{
		return total_times_;
	}

	inline void Timer::SetTotalRunTimes(int times)
	{
		total_times_ = times;
	}

	inline Duration Timer::GetInterval() const
	{
		return interval_;
	}

	inline void Timer::SetInterval(Duration interval)
	{
		interval_ = interval;
	}

	inline Timer::Callback Timer::GetCallback() const
	{
		return callback_;
	}

	inline void Timer::SetCallback(const Timer::Callback& callback)
	{
		callback_ = callback;
	}
}
