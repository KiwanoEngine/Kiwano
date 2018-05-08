#include "..\..\e2dshape.h"

e2d::Ellipse::Ellipse()
	: _fRadiusX(0)
	, _fRadiusY(0)
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
	return _fRadiusX;
}

double e2d::Ellipse::getRadiusY() const
{
	return _fRadiusY;
}

void e2d::Ellipse::setRadiusX(double radiusX)
{
	_fRadiusX = static_cast<float>(radiusX);
	Node::setWidth(radiusX * 2);
}

void e2d::Ellipse::setRadiusY(double radiusY)
{
	_fRadiusY = static_cast<float>(radiusY);
	Node::setHeight(radiusY * 2);
}

void e2d::Ellipse::_renderLine()
{
	Renderer::getRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(_fRadiusX, _fRadiusY), _fRadiusX, _fRadiusY),
		Renderer::getSolidColorBrush(),
		_fStrokeWidth
	);
}

void e2d::Ellipse::_renderFill()
{
	Renderer::getRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(_fRadiusX, _fRadiusY), _fRadiusX, _fRadiusY),
		Renderer::getSolidColorBrush()
	);
}
