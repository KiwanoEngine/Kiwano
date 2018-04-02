#include "..\..\eshape.h"

e2d::Shape::Shape()
	: m_nStyle(ShapeStyle::SOLID)
	, m_nFillColor(Color::WHITE)
	, m_nLineColor(Color::BLUE)
	, m_fStrokeWidth(1)
{
}

e2d::Shape::~Shape()
{
}

void e2d::Shape::onRender()
{
	switch (m_nStyle)
	{
	case ShapeStyle::FILL:
	{
		Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_nFillColor, m_fDisplayOpacity));
		this->_renderFill();

		Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_nLineColor, m_fDisplayOpacity));
		this->_renderLine();
		break;
	}

	case ShapeStyle::ROUND:
	{
		Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_nLineColor, m_fDisplayOpacity));
		this->_renderLine();
		break;
	}

	case ShapeStyle::SOLID:
	{
		Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_nFillColor, m_fDisplayOpacity));
		this->_renderFill();
		break;
	}

	default:
		break;
	}
}

UINT32 e2d::Shape::getFillColor() const
{
	return m_nFillColor;
}

UINT32 e2d::Shape::getLineColor() const
{
	return m_nLineColor;
}

double e2d::Shape::getStrokeWidth() const
{
	return m_fStrokeWidth;
}

int e2d::Shape::getStyle() const
{
	return m_nStyle;
}

void e2d::Shape::setFillColor(UINT32 fillColor)
{
	m_nFillColor = fillColor;
}

void e2d::Shape::setLineColor(UINT32 lineColor)
{
	m_nLineColor = lineColor;
}

void e2d::Shape::setStrokeWidth(double strokeWidth)
{
	m_fStrokeWidth = static_cast<float>(strokeWidth);
}

void e2d::Shape::setStyle(int style)
{
	m_nStyle = style;
}
