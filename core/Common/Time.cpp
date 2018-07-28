#include "..\e2dcommon.h"

using namespace std::chrono;


e2d::Time::Time()
{
}

e2d::Time::Time(const std::chrono::steady_clock::time_point& time)
	: _timePoint(time)
{
}

time_t e2d::Time::getTimeStamp() const
{
	auto& now = time_point_cast<milliseconds>(_timePoint).time_since_epoch();
	return static_cast<time_t>(now.count());
}

e2d::Time e2d::Time::operator+(Duration const & other) const
{
	return std::move(Time(_timePoint + milliseconds(other.milliseconds())));
}

e2d::Time & e2d::Time::operator+=(Duration const & other)
{
	_timePoint += milliseconds(other.milliseconds());
	return (*this);
}

e2d::Duration e2d::Time::operator-(Time const & other) const
{
	auto& ms = duration_cast<milliseconds>(_timePoint - other._timePoint);
	return std::move(Duration(ms));
}

e2d::Time e2d::Time::now()
{
	return std::move(Time(steady_clock::now()));
}
