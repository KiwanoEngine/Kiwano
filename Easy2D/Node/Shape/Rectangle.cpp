#include "..\..\easy2d.h"
#include "..\..\EasyX\easyx.h"


Rect::Rect() :
	m_nWidth(0),
	m_nHeight(0)
{
}

Rect::Rect(int x, int y, int width, int height) :
	Node(x, y),
	m_nWidth(width),
	m_nHeight(height)
{
}

Rect::~Rect()
{
}

void Rect::solidShape()
{
	solidrectangle(m_nX, m_nY, m_nX + m_nWidth, m_nY + m_nHeight);
}

void Rect::fillShape()
{
	fillrectangle(m_nX, m_nY, m_nX + m_nWidth, m_nY + m_nHeight);
}

void Rect::roundShape()
{
	rectangle(m_nX, m_nY, m_nX + m_nWidth, m_nY + m_nHeight);
}

int Rect::getWidth() const
{
	return m_nWidth;
}

int Rect::getHeight() const
{
	return m_nHeight;
}

void Rect::setWidth(int width)
{
	m_nWidth = width;
}

void Rect::setHeight(int height)
{
	m_nHeight = height;
}

void Rect::setSize(int width, int height)
{
	m_nWidth = width;
	m_nHeight = height;
}