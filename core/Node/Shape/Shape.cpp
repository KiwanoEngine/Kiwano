#include "..\..\e2dshape.h"

e2d::Shape::Shape()
	: m_nStyle(ShapeStyle::SOLID)
	, m_nFillColor(Color::WHITE)
	, m_nLineColor(Color::BLUE, 0.5)
	, m_fStrokeWidth(1)
{
}

e2d::Shape::~Shape()
{
}

void e2d::Shape::onRender()
{
	auto pBrush = Renderer::getSolidColorBrush();
	pBrush->SetOpacity(m_fDisplayOpacity);

	switch (m_nStyle)
	{
	case ShapeStyle::FILL:
	{
		pBrush->SetColor(m_nFillColor.toColorF());
		this->_renderFill();

		pBrush->SetColor(m_nLineColor.toColorF());
		this->_renderLine();
		break;
	}

	case ShapeStyle::ROUND:
	{
		pBrush->SetColor(m_nLineColor.toColorF());
		this->_renderLine();
		break;
	}

	case ShapeStyle::SOLID:
	{
		pBrush->SetColor(m_nFillColor.toColorF());
		this->_renderFill();
		break;
	}

	default:
		break;
	}
}

e2d::Color e2d::Shape::getFillColor() const
{
	return m_nFillColor;
}

e2d::Color e2d::Shape::getLineColor() const
{
	return m_nLineColor;
}

double e2d::Shape::getStrokeWidth() const
{
	return m_fStrokeWidth;
}

e2d::ShapeStyle e2d::Shape::getStyle() const
{
	return m_nStyle;
}

void e2d::Shape::setFillColor(Color fillColor)
{
	m_nFillColor = fillColor;
}

void e2d::Shape::setLineColor(Color lineColor)
{
	m_nLineColor = lineColor;
}

void e2d::Shape::setStrokeWidth(double strokeWidth)
{
	m_fStrokeWidth = static_cast<float>(strokeWidth);
}

void e2d::Shape::setStyle(ShapeStyle style)
{
	m_nStyle = style;
}
