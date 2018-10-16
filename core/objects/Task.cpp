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

#include "..\e2dobject.h"


easy2d::Task::Task(const Function & func, const String & name)
	: running_(true)
	, stopped_(false)
	, run_times_(0)
	, total_times_(-1)
	, delay_()
	, callback_(func)
	, name_(name)
{
}

easy2d::Task::Task(const Function & func, float delay, int times, const String & name)
	: running_(true)
	, stopped_(false)
	, run_times_(0)
	, delay_(std::max(delay, 0.f))
	, total_times_(times)
	, callback_(func)
	, name_(name)
{
}

void easy2d::Task::Start()
{
	running_ = true;
	last_time_ = Time::Now();
}

void easy2d::Task::Stop()
{
	running_ = false;
}

void easy2d::Task::Update()
{
	if (total_times_ == 0)
	{
		stopped_ = true;
		return;
	}

	++run_times_;
	last_time_ += delay_;

	if (callback_)
	{
		callback_();
	}

	if (run_times_ == total_times_)
	{
		stopped_ = true;
		return;
	}
}

void easy2d::Task::ResetTime()
{
	last_time_ = Time::Now();
}

bool easy2d::Task::IsReady() const
{
	if (running_)
	{
		if (delay_.Milliseconds() == 0)
		{
			return true;
		}
		if (Time::Now() - last_time_ >= delay_)
		{
			return true;
		}
	}
	return false;
}

bool easy2d::Task::IsRunning() const
{
	return running_;
}

const easy2d::String& easy2d::Task::GetName() const
{
	return name_;
}
