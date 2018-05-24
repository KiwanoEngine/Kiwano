#include "..\..\e2dshape.h"

e2d::EllipseShape::EllipseShape()
	: _radiusX(0)
	, _radiusY(0)
{
	this->setPivot(0.5, 0.5);
}

e2d::EllipseShape::EllipseShape(double radiusX, double radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setPivot(0.5, 0.5);
}

e2d::EllipseShape::EllipseShape(Point center, double radiusX, double radiusY)
{
	this->setRadiusX(radiusX);
	this->setRadiusY(radiusY);
	this->setPos(center);
	this->setPivot(0.5, 0.5);
}

e2d::EllipseShape::~EllipseShape()
{
}

double e2d::EllipseShape::getRadiusX() const
{
	return _radiusX;
}

double e2d::EllipseShape::getRadiusY() const
{
	return _radiusY;
}

void e2d::EllipseShape::setRadiusX(double radiusX)
{
	_radiusX = float(radiusX);
	Node::setWidth(radiusX * 2);
}

void e2d::EllipseShape::setRadiusY(double radiusY)
{
	_radiusY = float(radiusY);
	Node::setHeight(radiusY * 2);
}

void e2d::EllipseShape::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radiusX, _radiusY), _radiusX, _radiusY),
		Renderer::getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::EllipseShape::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_radiusX, _radiusY), _radiusX, _radiusY),
		Renderer::getSolidColorBrush()
	);
}
