#include "..\..\e2dshape.h"

e2d::Shape::Shape()
	: _nStyle(ShapeStyle::SOLID)
	, _nFillColor(Color::WHITE)
	, _nLineColor(Color::BLUE, 0.5)
	, _fStrokeWidth(1)
{
}

e2d::Shape::~Shape()
{
}

void e2d::Shape::onRender()
{
	auto pBrush = Renderer::getSolidColorBrush();
	pBrush->SetOpacity(_fDisplayOpacity);

	switch (_nStyle)
	{
	case ShapeStyle::FILL:
	{
		pBrush->SetColor(_nFillColor.toColorF());
		this->_renderFill();

		pBrush->SetColor(_nLineColor.toColorF());
		this->_renderLine();
		break;
	}

	case ShapeStyle::ROUND:
	{
		pBrush->SetColor(_nLineColor.toColorF());
		this->_renderLine();
		break;
	}

	case ShapeStyle::SOLID:
	{
		pBrush->SetColor(_nFillColor.toColorF());
		this->_renderFill();
		break;
	}

	default:
		break;
	}
}

e2d::Color e2d::Shape::getFillColor() const
{
	return _nFillColor;
}

e2d::Color e2d::Shape::getLineColor() const
{
	return _nLineColor;
}

double e2d::Shape::getStrokeWidth() const
{
	return _fStrokeWidth;
}

e2d::ShapeStyle e2d::Shape::getStyle() const
{
	return _nStyle;
}

void e2d::Shape::setFillColor(Color fillColor)
{
	_nFillColor = fillColor;
}

void e2d::Shape::setLineColor(Color lineColor)
{
	_nLineColor = lineColor;
}

void e2d::Shape::setStrokeWidth(double strokeWidth)
{
	_fStrokeWidth = static_cast<float>(strokeWidth);
}

void e2d::Shape::setStyle(ShapeStyle style)
{
	_nStyle = style;
}
