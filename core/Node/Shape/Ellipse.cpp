#include "..\..\eshape.h"

e2d::Ellipse::Ellipse()
	: m_fRadiusX(0)
	, m_fRadiusY(0)
{
	this->setPivot(0.5, 0.5);
}

e2d::Ellipse::Ellipse(double radiusX, double radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setPivot(0.5, 0.5);
}

e2d::Ellipse::Ellipse(Point center, double radiusX, double radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setPos(center);
	this->setPivot(0.5, 0.5);
}

e2d::Ellipse::Ellipse(double centerX, double centerY, double radiusX, double radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setPos(centerX, centerY);
	this->setPivot(0.5, 0.5);
}

e2d::Ellipse::~Ellipse()
{
}

double e2d::Ellipse::getRadiusX() const
{
	return m_fRadiusX;
}

double e2d::Ellipse::getRadiusY() const
{
	return m_fRadiusY;
}

void e2d::Ellipse::setRadiusX(double radiusX)
{
	m_fRadiusX = static_cast<float>(radiusX);
	Node::setWidth(radiusX * 2);
}

void e2d::Ellipse::setRadiusY(double radiusY)
{
	m_fRadiusY = static_cast<float>(radiusY);
	Node::setHeight(radiusY * 2);
}

void e2d::Ellipse::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(m_fRadiusX, m_fRadiusY), m_fRadiusX, m_fRadiusY),
		Renderer::getSolidColorBrush(),
		m_fStrokeWidth
	);
}

void e2d::Ellipse::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(m_fRadiusX, m_fRadiusY), m_fRadiusX, m_fRadiusY),
		Renderer::getSolidColorBrush()
	);
}
