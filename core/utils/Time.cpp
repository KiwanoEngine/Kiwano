#include "..\e2dutil.h"

using namespace std::chrono;


e2d::Time::Time()
{
}

time_t e2d::Time::GetTimeStamp() const
{
	auto& duration = time_point_cast<milliseconds>(time_).time_since_epoch();
	return static_cast<time_t>(duration.count());
}

bool e2d::Time::IsZero() const
{
	return time_.time_since_epoch().count() == 0LL;
}

e2d::Time e2d::Time::operator+(Duration const & other) const
{
	Time t;
	t.time_ = time_ + milliseconds(other.Milliseconds());
	return std::move(t);
}

e2d::Time e2d::Time::operator-(Duration const & other) const
{
	Time t;
	t.time_ = time_ - milliseconds(other.Milliseconds());
	return std::move(t);
}

e2d::Time & e2d::Time::operator+=(Duration const & other)
{
	time_ += milliseconds(other.Milliseconds());
	return (*this);
}

e2d::Time & e2d::Time::operator-=(Duration const &other)
{
	time_ -= milliseconds(other.Milliseconds());
	return (*this);
}

e2d::Duration e2d::Time::operator-(Time const & other) const
{
	auto ms = duration_cast<milliseconds>(time_ - other.time_).count();
	return std::move(Duration(static_cast<float>(ms) / 1000.f));
}

e2d::Time e2d::Time::Now()
{
	Time t;
	t.time_ = steady_clock::now();
	return std::move(t);
}
