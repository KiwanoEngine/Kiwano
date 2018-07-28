#include "..\..\e2dshape.h"

e2d::RectShape::RectShape()
{
}

e2d::RectShape::RectShape(Size size)
{
	this->setSize(size);
}

e2d::RectShape::RectShape(Point topLeft, Size size)
{
	this->setPivot(0, 0);
	this->setPos(topLeft);
	this->setSize(size);
}

e2d::RectShape::~RectShape()
{
}

void e2d::RectShape::_renderLine() const
{
	auto renderer = Renderer::getInstance();
	renderer->getRenderTarget()->DrawRectangle(
		D2D1::RectF(0, 0, _width, _height),
		renderer->getSolidColorBrush(),
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::RectShape::_renderFill() const
{
	auto renderer = Renderer::getInstance();
	renderer->getRenderTarget()->FillRectangle(
		D2D1::RectF(0, 0, _width, _height),
		renderer->getSolidColorBrush()
	);
}
