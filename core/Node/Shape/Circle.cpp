#include "..\..\e2dshape.h"

e2d::Circle::Circle()
	: _radius(0)
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

e2d::Circle * e2d::Circle::create()
{
	return GC::create<Circle>();
}

e2d::Circle * e2d::Circle::create(double radius)
{
	return GC::create<Circle>(radius);
}

e2d::Circle * e2d::Circle::create(Point center, double radius)
{
	return GC::create<Circle>(center, radius);
}

e2d::Circle::~Circle()
{
}

double e2d::Circle::getRadius() const
{
	return _radius;
}

void e2d::Circle::setRadius(double radius)
{
	_radius = float(radius);
	Node::setSize(radius * 2, radius * 2);
}

void e2d::Circle::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radius, _radius), _radius, _radius),
		Renderer::getSolidColorBrush(),
		_strokeWidth
	);
}

void e2d::Circle::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radius, _radius), _radius, _radius),
		Renderer::getSolidColorBrush()
	);
}
