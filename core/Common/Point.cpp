#include "..\e2dcommon.h"
#include <cmath>


e2d::Point::Point()
{
	x = 0;
	y = 0;
}

e2d::Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

e2d::Point::Point(const Point & other)
{
	x = other.x;
	y = other.y;
}

e2d::Point e2d::Point::operator+(Point const & p) const
{
	return Point(x + p.x, y + p.y);
}

e2d::Point e2d::Point::operator-(Point const & p) const
{
	return Point(x - p.x, y - p.y);
}

e2d::Point e2d::Point::operator*(double const & value) const
{
	return Point(x * value, y * value);
}

e2d::Point e2d::Point::operator/(double const & value) const
{
	return Point(x / value, y / value);
}

e2d::Point::operator e2d::Size() const
{
	return Size(x, y);
}

double e2d::Point::distance(const Point &p1, const Point &p2)
{
	return sqrt(
		(p1.x - p2.x) * (p1.x - p2.x) + 
		(p1.y - p2.y) * (p1.y - p2.y)
	);
}

e2d::Point e2d::Point::operator-() const
{
	return Point(-x, -y);
}

bool e2d::Point::operator==(const Point & point) const
{
	return (x == point.x) && (y == point.y);
}
