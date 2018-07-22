#include "..\e2dcommon.h"

using namespace std::chrono;

e2d::Duration::Duration()
	: _ms()
{
}

e2d::Duration::Duration(int ms)
	: _ms(ms)
{
}

e2d::Duration::Duration(std::chrono::milliseconds ms)
	: _ms(ms)
{
}

int e2d::Duration::milliseconds() const
{
	return static_cast<int>(_ms.count());
}

double e2d::Duration::seconds() const
{
	return _ms.count() / 1000.0;
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
	return std::move(Duration(_ms + other._ms));
}

e2d::Duration e2d::Duration::operator-(Duration const & other) const
{
	return std::move(Duration(_ms - other._ms));
}
