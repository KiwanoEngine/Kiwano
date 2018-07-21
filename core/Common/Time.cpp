#include "..\e2dcommon.h"

using namespace std::chrono;


e2d::Time::Time()
{
}

time_t e2d::Time::getTimeStamp() const
{
	auto now = time_point_cast<milliseconds>(time).time_since_epoch();
	return static_cast<time_t>(now.count());
}

e2d::Time e2d::Time::operator+(Duration const & other) const
{
	Time result;
	result.time = time - other.ms;
	return std::move(result);
}

e2d::Duration e2d::Time::operator-(Time const & other) const
{
	int ms = static_cast<int>(duration_cast<milliseconds>(time - other.time).count());
	return std::move(Duration(ms));
}

e2d::Time e2d::Time::now()
{
	Time now;
	now.time = steady_clock::now();
	return std::move(now);
}
