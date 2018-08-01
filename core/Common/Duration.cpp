#include "..\e2dcommon.h"

using namespace std::chrono;

e2d::Duration::Duration()
	: _ms()
{
}

e2d::Duration::Duration(float seconds)
	: _ms(static_cast<long long>(seconds * 1000.f))
{
}

int e2d::Duration::milliseconds() const
{
	return static_cast<int>(_ms.count());
}

float e2d::Duration::seconds() const
{
	return _ms.count() / 1000.f;
}

bool e2d::Duration::operator==(const Duration & other) const
{
	return _ms == other._ms;
}

bool e2d::Duration::operator!=(const Duration & other) const
{
	return _ms != other._ms;
}

bool e2d::Duration::operator>(const Duration & other) const
{
	return _ms > other._ms;
}

bool e2d::Duration::operator>=(const Duration & other) const
{
	return _ms >= other._ms;
}

bool e2d::Duration::operator<(const Duration & other) const
{
	return _ms < other._ms;
}

bool e2d::Duration::operator<=(const Duration & other) const
{
	return _ms <= other._ms;
}

e2d::Duration e2d::Duration::operator+(Duration const & other) const
{
	Duration d;
	d._ms = _ms + other._ms;
	return std::move(d);
}

e2d::Duration e2d::Duration::operator-(Duration const & other) const
{
	Duration d;
	d._ms = _ms - other._ms;
	return std::move(d);
}

e2d::Duration & e2d::Duration::operator+=(Duration const &other)
{
	_ms += other._ms;
	return (*this);
}

e2d::Duration & e2d::Duration::operator-=(Duration const &other)
{
	_ms -= other._ms;
	return (*this);
}
