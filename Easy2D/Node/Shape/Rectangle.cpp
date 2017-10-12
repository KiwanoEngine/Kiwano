#include "..\..\easy2d.h"
#include "..\..\EasyX\easyx.h"


Rect::Rect()
{
}

Rect::Rect(int x, int y, int width, int height) :
	m_Size(width, height)
{
	setPos(x, y);
}

Rect::~Rect()
{
}

void Rect::solidShape()
{
	solidrectangle(getX(), getY(), getX() + m_Size.cx, getY() + m_Size.cy);
}

void Rect::fillShape()
{
	fillrectangle(getX(), getY(), getX() + m_Size.cx, getY() + m_Size.cy);
}

void Rect::roundShape()
{
	rectangle(getX(), getY(), getX() + m_Size.cx, getY() + m_Size.cy);
}

int Rect::getWidth() const
{
	return m_Size.cx;
}

int Rect::getHeight() const
{
	return m_Size.cy;
}

void Rect::setWidth(int width)
{
	m_Size.cx = width;
}

void Rect::setHeight(int height)
{
	m_Size.cy = height;
}

void Rect::setSize(int width, int height)
{
	m_Size.cx = width;
	m_Size.cy = height;
}