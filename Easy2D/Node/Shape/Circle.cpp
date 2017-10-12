#include "..\..\easy2d.h"
#include "..\..\EasyX\easyx.h"


Circle::Circle() :
	m_nRadius(0)
{
}

Circle::Circle(int x, int y, int radius) :
	m_nRadius(radius)
{
	setPos(x, y);
}

Circle::~Circle()
{
}

void Circle::solidShape()
{
	solidcircle(getX(), getY(), m_nRadius);
}

void Circle::fillShape()
{
	fillcircle(getX(), getY(), m_nRadius);
}

void Circle::roundShape()
{
	circle(getX(), getY(), m_nRadius);
}

int Circle::getRadius() const
{
	return m_nRadius;
}

void Circle::setRadius(int r)
{
	m_nRadius = r;
}