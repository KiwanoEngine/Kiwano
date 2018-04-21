#include "..\..\e2dshape.h"

e2d::Circle::Circle()
	: m_fRadius(0)
{
	this->setPivot(0.5, 0.5);
}

e2d::Circle::Circle(double radius)
{
	this->setRadius(radius);
	this->setPivot(0.5, 0.5);
}

e2d::Circle::Circle(Point center, double radius)
{
	this->setRadius(radius);
	this->setPos(center);
	this->setPivot(0.5, 0.5);
}

e2d::Circle::Circle(double centerX, double centerY, double radius)
{
	this->setRadius(radius);
	this->setPos(centerX, centerY);
	this->setPivot(0.5, 0.5);
}

e2d::Circle::~Circle()
{
}

double e2d::Circle::getRadius() const
{
	return m_fRadius;
}

void e2d::Circle::setRadius(double radius)
{
	m_fRadius = static_cast<float>(radius);
	Node::setSize(radius * 2, radius * 2);
}

void e2d::Circle::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(m_fRadius, m_fRadius), m_fRadius, m_fRadius),
		Renderer::getSolidColorBrush(),
		m_fStrokeWidth
	);
}

void e2d::Circle::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(m_fRadius, m_fRadius), m_fRadius, m_fRadius),
		Renderer::getSolidColorBrush()
	);
}
