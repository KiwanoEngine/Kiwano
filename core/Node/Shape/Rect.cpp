#include "..\..\e2dshape.h"

e2d::Rect::Rect()
{
}

e2d::Rect::Rect(Size size)
{
	this->setSize(size);
}

e2d::Rect::Rect(Point topLeft, Size size)
{
	this->setPivot(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

e2d::Rect * e2d::Rect::create()
{
	return Create<Rect>();
}

e2d::Rect * e2d::Rect::create(Size size)
{
	return Create<Rect>(size);
}

e2d::Rect * e2d::Rect::create(Point topLeft, Size size)
{
	return Create<Rect>(topLeft, size);
}

e2d::Rect::~Rect()
{
}

void e2d::Rect::_renderLine()
{
	Renderer::getRenderTarget()->DrawRectangle(
		D2D1::RectF(0, 0, _width, _height),
		Renderer::getSolidColorBrush(),
		_strokeWidth
	);
}

void e2d::Rect::_renderFill()
{
	Renderer::getRenderTarget()->FillRectangle(
		D2D1::RectF(0, 0, _width, _height),
		Renderer::getSolidColorBrush()
	);
}
