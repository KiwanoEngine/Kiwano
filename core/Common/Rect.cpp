#include "..\e2dcommon.h"

e2d::Rect::Rect(void)
{
	setRect(0.0, 0.0, 0.0, 0.0);
}

e2d::Rect::Rect(double x, double y, double width, double height)
{
	setRect(x, y, width, height);
}

e2d::Rect::Rect(const Point& pos, const Size& size)
{
	setRect(pos.x, pos.y, size.width, size.height);
}

e2d::Rect::Rect(const Rect& other)
{
	setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

e2d::Rect& e2d::Rect::operator= (const Rect& other)
{
	setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
	return *this;
}

bool e2d::Rect::operator==(const Rect & rect) const
{
	return (origin == rect.origin) && (size == rect.size);
}

void e2d::Rect::setRect(double x, double y, double width, double height)
{
	origin.x = x;
	origin.y = y;

	size.width = width;
	size.height = height;
}

bool e2d::Rect::containsPoint(const Point& point) const
{
	if (point.x >= origin.x && point.x <= (origin.y + size.height)
		&& point.y >= origin.y && point.y <= (origin.y + size.height))
	{
		return true;
	}
	return false;
}

bool e2d::Rect::intersects(const Rect& rect) const
{
	return !((origin.x + size.width)			< rect.origin.x ||
			(rect.origin.x + rect.size.width)	<      origin.x ||
			(origin.y + size.height)			< rect.origin.y ||
			(rect.origin.y + rect.size.height)	<      origin.y);
}
