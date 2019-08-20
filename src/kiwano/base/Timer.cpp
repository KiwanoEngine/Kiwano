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

#include "Timer.h"

namespace kiwano
{
	Timer::Timer(Callback const& func, Duration delay, Int32 times, String const& name)
		: running_(true)
		, run_times_(0)
		, total_times_(times)
		, delay_(delay)
		, callback_(func)
		, delta_()
	{
		SetName(name);
	}

	void Timer::Start()
	{
		running_ = true;
	}

	void Timer::Stop()
	{
		running_ = false;
	}

	void Timer::Update(Duration dt, bool& remove_after_update)
	{
		if (!running_)
			return;

		if (total_times_ == 0)
		{
			remove_after_update = true;
			return;
		}

		if (!delay_.IsZero())
		{
			delta_ += dt;
			if (delta_ < delay_)
				return;
		}

		++run_times_;

		if (callback_)
		{
			callback_();
		}

		if (run_times_ == total_times_)
		{
			remove_after_update = true;
			return;
		}
	}

	void Timer::Reset()
	{
		delta_ = Duration{};
		run_times_ = 0;
	}

	bool Timer::IsRunning() const
	{
		return running_;
	}

}
