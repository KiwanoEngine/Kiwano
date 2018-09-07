#include "..\e2dobject.h"


e2d::Task::Task(const Function & func, const String & name)
	: running_(true)
	, stopped_(false)
	, run_times_(0)
	, total_times_(-1)
	, delay_()
	, callback_(func)
	, name_(name)
{
}

e2d::Task::Task(const Function & func, float delay, int times, const String & name)
	: running_(true)
	, stopped_(false)
	, run_times_(0)
	, delay_(std::max(delay, 0.f))
	, total_times_(times)
	, callback_(func)
	, name_(name)
{
}

void e2d::Task::Start()
{
	running_ = true;
	last_time_ = Time::Now();
}

void e2d::Task::Stop()
{
	running_ = false;
}

void e2d::Task::Update()
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

void e2d::Task::ResetTime()
{
	last_time_ = Time::Now();
}

bool e2d::Task::IsReady() const
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

bool e2d::Task::IsRunning() const
{
	return running_;
}

const e2d::String& e2d::Task::GetName() const
{
	return name_;
}
