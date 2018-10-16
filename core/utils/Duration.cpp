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

easy2d::Duration::Duration()
	: duration_ms_()
{
}

easy2d::Duration::Duration(float seconds)
	: duration_ms_(static_cast<long long>(seconds * 1000.f))
{
}

int easy2d::Duration::Milliseconds() const
{
	return static_cast<int>(duration_ms_.count());
}

float easy2d::Duration::Seconds() const
{
	return duration_ms_.count() / 1000.f;
}

bool easy2d::Duration::operator==(const Duration & other) const
{
	return duration_ms_ == other.duration_ms_;
}

bool easy2d::Duration::operator!=(const Duration & other) const
{
	return duration_ms_ != other.duration_ms_;
}

bool easy2d::Duration::operator>(const Duration & other) const
{
	return duration_ms_ > other.duration_ms_;
}

bool easy2d::Duration::operator>=(const Duration & other) const
{
	return duration_ms_ >= other.duration_ms_;
}

bool easy2d::Duration::operator<(const Duration & other) const
{
	return duration_ms_ < other.duration_ms_;
}

bool easy2d::Duration::operator<=(const Duration & other) const
{
	return duration_ms_ <= other.duration_ms_;
}

easy2d::Duration easy2d::Duration::operator+(Duration const & other) const
{
	Duration d;
	d.duration_ms_ = duration_ms_ + other.duration_ms_;
	return std::move(d);
}

easy2d::Duration easy2d::Duration::operator-(Duration const & other) const
{
	Duration d;
	d.duration_ms_ = duration_ms_ - other.duration_ms_;
	return std::move(d);
}

easy2d::Duration & easy2d::Duration::operator+=(Duration const &other)
{
	duration_ms_ += other.duration_ms_;
	return (*this);
}

easy2d::Duration & easy2d::Duration::operator-=(Duration const &other)
{
	duration_ms_ -= other.duration_ms_;
	return (*this);
}
