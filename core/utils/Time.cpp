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

#include "..\e2dutil.h"

using namespace std::chrono;


easy2d::Time::Time()
{
}

easy2d::Time::Time(std::chrono::steady_clock::time_point time)
	: time_(time)
{
}

easy2d::Time::Time(const Time & other)
	: time_(other.time_)
{
}

easy2d::Time::Time(Time && other)
	: time_(std::move(other.time_))
{
}

time_t easy2d::Time::GetTimeStamp() const
{
	auto& duration = time_point_cast<milliseconds>(time_).time_since_epoch();
	return static_cast<time_t>(duration.count());
}

bool easy2d::Time::IsZero() const
{
	return time_.time_since_epoch().count() == 0LL;
}

easy2d::Time easy2d::Time::operator+(const Duration & other) const
{
	return Time(time_ + milliseconds(other.Milliseconds()));
}

easy2d::Time easy2d::Time::operator-(const Duration & other) const
{
	return Time(time_ - milliseconds(other.Milliseconds()));
}

easy2d::Time & easy2d::Time::operator+=(const Duration & other)
{
	time_ += milliseconds(other.Milliseconds());
	return (*this);
}

easy2d::Time & easy2d::Time::operator-=(const Duration &other)
{
	time_ -= milliseconds(other.Milliseconds());
	return (*this);
}

easy2d::Duration easy2d::Time::operator-(const Time & other) const
{
	auto ms = duration_cast<milliseconds>(time_ - other.time_).count();
	return Duration(static_cast<int>(ms));
}

easy2d::Time& easy2d::Time::operator=(const Time & other) E2D_NOEXCEPT
{
	if (this == &other)
		return *this;

	time_ = other.time_;
	return *this;
}

easy2d::Time& easy2d::Time::operator=(Time && other) E2D_NOEXCEPT
{
	if (this == &other)
		return *this;

	time_ = std::move(other.time_);
	return *this;
}

easy2d::Time easy2d::Time::Now()
{
	return Time(steady_clock::now());
}
