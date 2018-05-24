#include "..\..\e2dshape.h"

e2d::RoundRectShape::RoundRectShape()
	: _radiusX(0)
	, _radiusY(0)
{
}

e2d::RoundRectShape::RoundRectShape(Size size, double radiusX, double radiusY)
	: _radiusX(float(radiusX))
	, _radiusY(float(radiusY))
{
	this->setSize(size);
}

e2d::RoundRectShape::RoundRectShape(Point topLeft, Size size, double radiusX, double radiusY)
	: _radiusX(float(radiusX))
	, _radiusY(float(radiusY))
{
	this->setPivot(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

e2d::RoundRectShape::~RoundRectShape()
{
}

double e2d::RoundRectShape::getRadiusX() const
{
	return _radiusX;
}

double e2d::RoundRectShape::getRadiusY() const
{
	return _radiusY;
}

void e2d::RoundRectShape::setRadiusX(double radiusX)
{
	_radiusX = float(radiusX);
}

void e2d::RoundRectShape::setRadiusY(double radiusY)
{
	_radiusY = float(radiusY);
}

void e2d::RoundRectShape::_renderLine()
{
	Renderer::getRenderTarget()->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		Renderer::getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::RoundRectShape::_renderFill()
{
	Renderer::getRenderTarget()->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		Renderer::getSolidColorBrush()
	);
}
