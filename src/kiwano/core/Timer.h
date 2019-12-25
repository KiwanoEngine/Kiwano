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
    /// @brief 定时器
	/// @details 定时器用于每隔一段时间执行一次回调函数，且可以指定执行总次数
	class KGE_API Timer
		: public ObjectBase
		, protected IntrusiveListItem<TimerPtr>
	{
		friend class TimerManager;
		friend IntrusiveList<TimerPtr>;

	public:
		/// \~chinese
		/// @brief 定时器回调函数
		/// @details 回调函数第一个参数是定时器自身，第二个参数是距离上次更新的时间间隔
		using Callback = Function<void(Timer* /* self */, Duration /* dt */)>;

		/// \~chinese
		/// @brief 构造空定时器
		Timer();

		/// \~chinese
		/// @brief 构造定时器
		/// @param cb 回调函数
		/// @param interval 时间间隔
		/// @param times 执行次数（设 -1 为永久执行）
		Timer(Callback const& cb, Duration interval, int times = -1);

		/// \~chinese
		/// @brief 构造定时器
		/// @param name 名称
		/// @param cb 回调函数
		/// @param interval 时间间隔
		/// @param times 执行次数（设 -1 为永久执行）
		Timer(String const& name, Callback const& cb, Duration interval, int times = -1);

		/// \~chinese
		/// @brief 启动定时器
		void Start();

		/// \~chinese
		/// @brief 停止定时器
		void Stop();

		/// \~chinese
		/// @brief 移除定时器
		void Remove();

		/// \~chinese
		/// @brief 定时器是否在运行
		bool IsRunning() const;

		/// \~chinese
		/// @brief 定时器是否可移除
		bool IsRemoveable() const;

		/// \~chinese
		/// @brief 获取定时器执行过回调函数的次数
		int GetRunTimes() const;

		/// \~chinese
		/// @brief 获取定时器执行回调函数的总次数
		int GetTotalRunTimes() const;

		/// \~chinese
		/// @brief 设置定时器执行回调函数的总次数
		void SetTotalRunTimes(int times);

		/// \~chinese
		/// @brief 获取定时器执行时间间隔
		Duration GetInterval() const;

		/// \~chinese
		/// @brief 设置定时器执行时间间隔
		void SetInterval(Duration interval);

		/// \~chinese
		/// @brief 获取定时器回调函数
		Callback GetCallback() const;

		/// \~chinese
		/// @brief 设置定时器回调函数
		void SetCallback(const Callback& callback);

	private:
		/// \~chinese
		/// @brief 更新定时器
		void Update(Duration dt);

		/// \~chinese
		/// @brief 重置定时器
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
