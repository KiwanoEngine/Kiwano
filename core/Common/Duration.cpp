#include "..\e2dcommon.h"

using namespace std::chrono;

e2d::Duration::Duration()
	: ms()
{
}

e2d::Duration::Duration(int ms)
	: ms(ms)
{
}

int e2d::Duration::milliseconds() const
{
	return static_cast<int>(ms.count());
}

double e2d::Duration::seconds() const
{
	return ms.count() / 1000.0;
}

double e2d::Duration::minutes() const
{
	return ms.count() / 1000.0 / 60.0;
}

bool e2d::Duration::operator==(const Duration & other) const
{
	return ms == other.ms;
}

bool e2d::Duration::operator!=(const Duration & other) const
{
	return ms != other.ms;
}

bool e2d::Duration::operator>(const Duration & other) const
{
	return ms > other.ms;
}

bool e2d::Duration::operator>=(const Duration & other) const
{
	return ms >= other.ms;
}

bool e2d::Duration::operator<(const Duration & other) const
{
	return ms < other.ms;
}

bool e2d::Duration::operator<=(const Duration & other) const
{
	return ms <= other.ms;
}

e2d::Duration e2d::Duration::operator+(Duration const & other) const
{
	Duration result;
	result.ms = ms + other.ms;
	return std::move(result);
}

e2d::Duration e2d::Duration::operator-(Duration const & other) const
{
	Duration result;
	result.ms = ms - other.ms;
	return std::move(result);
}
