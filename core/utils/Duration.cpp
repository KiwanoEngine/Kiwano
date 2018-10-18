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


const easy2d::Duration easy2d::Duration::Millisecond = easy2d::Duration(1);
const easy2d::Duration easy2d::Duration::Second = 1000 * easy2d::Duration::Millisecond;
const easy2d::Duration easy2d::Duration::Minute = 60 * easy2d::Duration::Second;
const easy2d::Duration easy2d::Duration::Hour = 60 * easy2d::Duration::Minute;

easy2d::Duration::Duration()
	: milliseconds_(0)
{
}

easy2d::Duration::Duration(int64_t milliseconds)
	: milliseconds_(milliseconds)
{
}

int64_t easy2d::Duration::Milliseconds() const
{
	return milliseconds_;
}

float easy2d::Duration::Seconds() const
{
	int64_t sec = milliseconds_ / Second.milliseconds_;
	int64_t ms = milliseconds_ % Second.milliseconds_;
	return static_cast<float>(sec) + static_cast<float>(ms) / 1000.f;
}

float easy2d::Duration::Minutes() const
{
	int64_t min = milliseconds_ / Minute.milliseconds_;
	int64_t ms = milliseconds_ % Minute.milliseconds_;
	return static_cast<float>(min) + static_cast<float>(ms) / (60 * 1000.f);
}

float easy2d::Duration::Hours() const
{
	int64_t hour = milliseconds_ / Hour.milliseconds_;
	int64_t ms = milliseconds_ % Hour.milliseconds_;
	return static_cast<float>(hour) + static_cast<float>(ms) / (60 * 60 * 1000.f);
}

bool easy2d::Duration::operator==(const Duration & other) const
{
	return milliseconds_ == other.milliseconds_;
}

bool easy2d::Duration::operator!=(const Duration & other) const
{
	return milliseconds_ != other.milliseconds_;
}

bool easy2d::Duration::operator>(const Duration & other) const
{
	return milliseconds_ > other.milliseconds_;
}

bool easy2d::Duration::operator>=(const Duration & other) const
{
	return milliseconds_ >= other.milliseconds_;
}

bool easy2d::Duration::operator<(const Duration & other) const
{
	return milliseconds_ < other.milliseconds_;
}

bool easy2d::Duration::operator<=(const Duration & other) const
{
	return milliseconds_ <= other.milliseconds_;
}

easy2d::Duration easy2d::Duration::operator+(Duration const & other) const
{
	Duration d(milliseconds_ + other.milliseconds_);
	return std::move(d);
}

easy2d::Duration easy2d::Duration::operator-(Duration const & other) const
{
	Duration d(milliseconds_ - other.milliseconds_);
	return std::move(d);
}

easy2d::Duration easy2d::Duration::operator*(int value) const
{
	Duration d(milliseconds_ * value);
	return std::move(d);
}

easy2d::Duration easy2d::Duration::operator/(int value) const
{
	Duration d(milliseconds_ / value);
	return std::move(d);
}

easy2d::Duration easy2d::Duration::operator*(float value) const
{
	int64_t ms = static_cast<int64_t>(milliseconds_ * value);
	Duration d(ms);
	return std::move(d);
}

easy2d::Duration easy2d::Duration::operator/(float value) const
{
	int64_t ms = static_cast<int64_t>(milliseconds_ / value);
	Duration d(ms);
	return std::move(d);
}

easy2d::Duration & easy2d::Duration::operator+=(Duration const &other)
{
	milliseconds_ += other.milliseconds_;
	return (*this);
}

easy2d::Duration & easy2d::Duration::operator-=(Duration const &other)
{
	milliseconds_ -= other.milliseconds_;
	return (*this);
}

easy2d::Duration & easy2d::Duration::operator*=(int value)
{
	milliseconds_ *= value;
	return (*this);
}

easy2d::Duration & easy2d::Duration::operator/=(int value)
{
	milliseconds_ /= value;
	return (*this);
}

easy2d::Duration & easy2d::Duration::operator*=(float value)
{
	milliseconds_ *= value;
	return (*this);
}

easy2d::Duration & easy2d::Duration::operator/=(float value)
{
	milliseconds_ /= value;
	return (*this);
}
