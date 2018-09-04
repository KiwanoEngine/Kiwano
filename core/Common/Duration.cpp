#include "..\e2dcommon.h"

using namespace std::chrono;

e2d::Duration::Duration()
	: duration_ms_()
{
}

e2d::Duration::Duration(float seconds)
	: duration_ms_(static_cast<long long>(seconds * 1000.f))
{
}

int e2d::Duration::Milliseconds() const
{
	return static_cast<int>(duration_ms_.count());
}

float e2d::Duration::Seconds() const
{
	return duration_ms_.count() / 1000.f;
}

bool e2d::Duration::operator==(const Duration & other) const
{
	return duration_ms_ == other.duration_ms_;
}

bool e2d::Duration::operator!=(const Duration & other) const
{
	return duration_ms_ != other.duration_ms_;
}

bool e2d::Duration::operator>(const Duration & other) const
{
	return duration_ms_ > other.duration_ms_;
}

bool e2d::Duration::operator>=(const Duration & other) const
{
	return duration_ms_ >= other.duration_ms_;
}

bool e2d::Duration::operator<(const Duration & other) const
{
	return duration_ms_ < other.duration_ms_;
}

bool e2d::Duration::operator<=(const Duration & other) const
{
	return duration_ms_ <= other.duration_ms_;
}

e2d::Duration e2d::Duration::operator+(Duration const & other) const
{
	Duration d;
	d.duration_ms_ = duration_ms_ + other.duration_ms_;
	return std::move(d);
}

e2d::Duration e2d::Duration::operator-(Duration const & other) const
{
	Duration d;
	d.duration_ms_ = duration_ms_ - other.duration_ms_;
	return std::move(d);
}

e2d::Duration & e2d::Duration::operator+=(Duration const &other)
{
	duration_ms_ += other.duration_ms_;
	return (*this);
}

e2d::Duration & e2d::Duration::operator-=(Duration const &other)
{
	duration_ms_ -= other.duration_ms_;
	return (*this);
}
