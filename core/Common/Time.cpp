#include "..\e2dcommon.h"

using namespace std::chrono;


e2d::Time::Time()
{
}

e2d::Time e2d::Time::operator+(Duration const & size) const
{
	return Time();
}

e2d::Duration e2d::Time::operator-(Time const & size) const
{
	return Duration();
}
