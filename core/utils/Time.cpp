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

time_t easy2d::Time::GetTimeStamp() const
{
	auto& duration = time_point_cast<milliseconds>(time_).time_since_epoch();
	return static_cast<time_t>(duration.count());
}

bool easy2d::Time::IsZero() const
{
	return time_.time_since_epoch().count() == 0LL;
}

easy2d::Time easy2d::Time::operator+(Duration const & other) const
{
	Time t;
	t.time_ = time_ + milliseconds(other.Milliseconds());
	return std::move(t);
}

easy2d::Time easy2d::Time::operator-(Duration const & other) const
{
	Time t;
	t.time_ = time_ - milliseconds(other.Milliseconds());
	return std::move(t);
}

easy2d::Time & easy2d::Time::operator+=(Duration const & other)
{
	time_ += milliseconds(other.Milliseconds());
	return (*this);
}

easy2d::Time & easy2d::Time::operator-=(Duration const &other)
{
	time_ -= milliseconds(other.Milliseconds());
	return (*this);
}

easy2d::Duration easy2d::Time::operator-(Time const & other) const
{
	auto ms = duration_cast<milliseconds>(time_ - other.time_).count();
	Duration d(static_cast<int>(ms));
	return std::move(d);
}

easy2d::Time easy2d::Time::Now()
{
	Time t;
	t.time_ = steady_clock::now();
	return std::move(t);
}
