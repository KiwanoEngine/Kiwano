#include "..\..\e2dshape.h"

e2d::CircleShape::CircleShape()
	: _radius(0)
{
	this->setPivot(0.5, 0.5);
}

e2d::CircleShape::CircleShape(float radius)
{
	this->setRadius(radius);
	this->setPivot(0.5, 0.5);
}

e2d::CircleShape::CircleShape(Point center, float radius)
{
	this->setRadius(radius);
	this->setPos(center);
	this->setPivot(0.5, 0.5);
}

e2d::CircleShape::~CircleShape()
{
}

float e2d::CircleShape::getRadius() const
{
	return _radius;
}

void e2d::CircleShape::setRadius(float radius)
{
	_radius = radius;
	Node::setSize(radius * 2, radius * 2);
}

void e2d::CircleShape::_renderLine() const
{
	auto renderer = Renderer::getInstance();
	renderer->getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radius, _radius), _radius, _radius),
		renderer->getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::CircleShape::_renderFill() const
{
	auto renderer = Renderer::getInstance();
	renderer->getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radius, _radius), _radius, _radius),
		renderer->getSolidColorBrush()
	);
}
