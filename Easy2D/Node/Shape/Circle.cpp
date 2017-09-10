#include "..\..\easy2d.h"
#include "..\..\EasyX\easyx.h"


Circle::Circle() :
	m_nRadius(0)
{
}

Circle::Circle(int x, int y, int radius) :
	Node(x, y),
	m_nRadius(radius)
{
}

Circle::~Circle()
{
}

void Circle::solidShape()
{
	solidcircle(m_nX, m_nY, m_nRadius);
}

void Circle::fillShape()
{
	fillcircle(m_nX, m_nY, m_nRadius);
}

void Circle::roundShape()
{
	circle(m_nX, m_nY, m_nRadius);
}

int Circle::getRadius() const
{
	return m_nRadius;
}

void Circle::setRadius(int r)
{
	m_nRadius = r;
}