#include "..\..\e2dshape.h"

e2d::Circle::Circle()
	: _fRadius(0)
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
	return _fRadius;
}

void e2d::Circle::setRadius(double radius)
{
	_fRadius = static_cast<float>(radius);
	Node::setSize(radius * 2, radius * 2);
}

void e2d::Circle::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_fRadius, _fRadius), _fRadius, _fRadius),
		Renderer::getSolidColorBrush(),
		_fStrokeWidth
	);
}

void e2d::Circle::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_fRadius, _fRadius), _fRadius, _fRadius),
		Renderer::getSolidColorBrush()
	);
}
