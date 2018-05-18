#include "..\..\e2dshape.h"

e2d::RoundRect::RoundRect()
	: _radiusX(0)
	, _radiusY(0)
{
}

e2d::RoundRect::RoundRect(Size size, double radiusX, double radiusY)
	: _radiusX(float(radiusX))
	, _radiusY(float(radiusY))
{
	this->setSize(size);
}

e2d::RoundRect::RoundRect(Point topLeft, Size size, double radiusX, double radiusY)
	: _radiusX(float(radiusX))
	, _radiusY(float(radiusY))
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
	return _radiusX;
}

double e2d::RoundRect::getRadiusY() const
{
	return _radiusY;
}

void e2d::RoundRect::setRadiusX(double radiusX)
{
	_radiusX = float(radiusX);
}

void e2d::RoundRect::setRadiusY(double radiusY)
{
	_radiusY = float(radiusY);
}

void e2d::RoundRect::_renderLine()
{
	Renderer::getRenderTarget()->DrawRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		Renderer::getSolidColorBrush(),
		_strokeWidth
	);
}

void e2d::RoundRect::_renderFill()
{
	Renderer::getRenderTarget()->FillRoundedRectangle(
		D2D1::RoundedRect(D2D1::RectF(0, 0, _width, _height), _radiusX, _radiusY),
		Renderer::getSolidColorBrush()
	);
}
