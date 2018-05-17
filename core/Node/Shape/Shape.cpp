#include "..\..\e2dshape.h"

e2d::Shape::Shape()
	: _style(ShapeStyle::SOLID)
	, _fillColor(Color::BLUE, 0.3)
	, _lineColor(Color::BLUE, 0.5)
	, _strokeWidth(1)
{
}

e2d::Shape::~Shape()
{
}

void e2d::Shape::onRender()
{
	auto pBrush = Renderer::getSolidColorBrush();
	pBrush->SetOpacity(_displayOpacity);

	switch (_style)
	{
	case ShapeStyle::FILL:
	{
		pBrush->SetColor(_fillColor.toColorF());
		this->_renderFill();

		pBrush->SetColor(_lineColor.toColorF());
		this->_renderLine();
		break;
	}

	case ShapeStyle::ROUND:
	{
		pBrush->SetColor(_lineColor.toColorF());
		this->_renderLine();
		break;
	}

	case ShapeStyle::SOLID:
	{
		pBrush->SetColor(_fillColor.toColorF());
		this->_renderFill();
		break;
	}

	default:
		break;
	}
}

e2d::Color e2d::Shape::getFillColor() const
{
	return _fillColor;
}

e2d::Color e2d::Shape::getLineColor() const
{
	return _lineColor;
}

double e2d::Shape::getStrokeWidth() const
{
	return _strokeWidth;
}

e2d::ShapeStyle e2d::Shape::getStyle() const
{
	return _style;
}

void e2d::Shape::setFillColor(Color fillColor)
{
	_fillColor = fillColor;
}

void e2d::Shape::setLineColor(Color lineColor)
{
	_lineColor = lineColor;
}

void e2d::Shape::setStrokeWidth(double strokeWidth)
{
	_strokeWidth = float(strokeWidth);
}

void e2d::Shape::setStyle(ShapeStyle style)
{
	_style = style;
}
