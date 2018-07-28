#include "..\..\e2dshape.h"

e2d::RoundRectShape::RoundRectShape()
	: _radiusX(0)
	, _radiusY(0)
{
}

e2d::RoundRectShape::RoundRectShape(Size size, float radiusX, float radiusY)
	: _radiusX(radiusX)
	, _radiusY(radiusY)
{
	this->setSize(size);
}

e2d::RoundRectShape::RoundRectShape(Point topLeft, Size size, float radiusX, float radiusY)
	: _radiusX(radiusX)
	, _radiusY(radiusY)
{
	this->setPivot(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

e2d::RoundRectShape::~RoundRectShape()
{
}

float e2d::RoundRectShape::getRadiusX() const
{
	return _radiusX;
}

float e2d::RoundRectShape::getRadiusY() const
{
	return _radiusY;
}

void e2d::RoundRectShape::setRadiusX(float radiusX)
{
	_radiusX = radiusX;
}

void e2d::RoundRectShape::setRadiusY(float radiusY)
{
	_radiusY = radiusY;
}

void e2d::RoundRectShape::_renderLine() const
{
	auto renderer = Renderer::getInstance();
	renderer->getRenderTarget()->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		renderer->getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::RoundRectShape::_renderFill() const
{
	auto renderer = Renderer::getInstance();
	renderer->getRenderTarget()->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		renderer->getSolidColorBrush()
	);
}
