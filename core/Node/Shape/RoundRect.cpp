#include "..\..\e2dshape.h"

e2d::RoundRect::RoundRect()
	: m_fRadiusX(0)
	, m_fRadiusY(0)
{
}

e2d::RoundRect::RoundRect(double width, double height, double radiusX, double radiusY)
	: m_fRadiusX(static_cast<float>(radiusX))
	, m_fRadiusY(static_cast<float>(radiusY))
{
	this->setSize(width, height);
}

e2d::RoundRect::RoundRect(Size size, double radiusX, double radiusY)
	: m_fRadiusX(static_cast<float>(radiusX))
	, m_fRadiusY(static_cast<float>(radiusY))
{
	this->setSize(size);
}

e2d::RoundRect::RoundRect(double top, double left, double width, double height, double radiusX, double radiusY)
	: m_fRadiusX(static_cast<float>(radiusX))
	, m_fRadiusY(static_cast<float>(radiusY))
{
	this->setPivot(0, 0);
	this->setPos(top, left);
	this->setSize(width, height);
}

e2d::RoundRect::RoundRect(Point topLeft, Size size, double radiusX, double radiusY)
	: m_fRadiusX(static_cast<float>(radiusX))
	, m_fRadiusY(static_cast<float>(radiusY))
{
	this->setPivot(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

e2d::RoundRect::~RoundRect()
{
}

double e2d::RoundRect::getRadiusX() const
{
	return m_fRadiusX;
}

double e2d::RoundRect::getRadiusY() const
{
	return m_fRadiusY;
}

void e2d::RoundRect::setRadiusX(double radiusX)
{
	m_fRadiusX = static_cast<float>(radiusX);
}

void e2d::RoundRect::setRadiusY(double radiusY)
{
	m_fRadiusY = static_cast<float>(radiusY);
}

void e2d::RoundRect::_renderLine()
{
	Renderer::getRenderTarget()->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, m_fWidth, m_fHeight), m_fRadiusX, m_fRadiusY),
		Renderer::getSolidColorBrush(),
		m_fStrokeWidth
	);
}

void e2d::RoundRect::_renderFill()
{
	Renderer::getRenderTarget()->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, m_fWidth, m_fHeight), m_fRadiusX, m_fRadiusY),
		Renderer::getSolidColorBrush()
	);
}
