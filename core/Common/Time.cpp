#include "..\e2dcommon.h"

using namespace std::chrono;


e2d::Time::Time()
{
}

time_t e2d::Time::getTimeStamp() const
{
	auto& duration = time_point_cast<milliseconds>(_timePoint).time_since_epoch();
	return static_cast<time_t>(duration.count());
}

bool e2d::Time::isZero() const
{
	return _timePoint.time_since_epoch().count() == 0LL;
}

e2d::Time e2d::Time::operator+(Duration const & other) const
{
	Time t;
	t._timePoint = _timePoint + milliseconds(other.milliseconds());
	return std::move(t);
}

e2d::Time e2d::Time::operator-(Duration const & other) const
{
	Time t;
	t._timePoint = _timePoint - milliseconds(other.milliseconds());
	return std::move(t);
}

e2d::Time & e2d::Time::operator+=(Duration const & other)
{
	_timePoint += milliseconds(other.milliseconds());
	return (*this);
}

e2d::Time & e2d::Time::operator-=(Duration const &other)
{
	_timePoint -= milliseconds(other.milliseconds());
	return (*this);
}

e2d::Duration e2d::Time::operator-(Time const & other) const
{
	auto ms = duration_cast<milliseconds>(_timePoint - other._timePoint).count();
	return std::move(Duration(static_cast<float>(ms) / 1000.f));
}

e2d::Time e2d::Time::now()
{
	Time t;
	t._timePoint = steady_clock::now();
	return std::move(t);
}
