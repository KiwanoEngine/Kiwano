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

#include "Task.h"

namespace easy2d
{
	Task::Task(Callback const& func, std::wstring const& name)
		: Task(func, Duration{}, -1, name)
	{
	}

	Task::Task(Callback const& func, Duration const& delay, int times, std::wstring const& name)
		: running_(true)
		, run_times_(0)
		, total_times_(times)
		, delay_(delay)
		, callback_(func)
		, name_(name)
		, delta_()
	{
	}

	void Task::Start()
	{
		running_ = true;
	}

	void Task::Stop()
	{
		running_ = false;
	}

	void Task::Update(Duration const& dt, bool& remove_after_update)
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

	void Task::Reset()
	{
		delta_ = Duration{};
		run_times_ = 0;
	}

	bool Task::IsRunning() const
	{
		return running_;
	}

	std::wstring const& Task::GetName() const
	{
		return name_;
	}

}