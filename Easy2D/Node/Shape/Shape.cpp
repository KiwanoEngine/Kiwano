#include "..\..\Easy2d.h"
#include "..\..\EasyX\easyx.h"

Shape::Shape() :
	lineColor(Color::black),
	fillColor(Color::white),
	m_eStyle(SOLID)
{
}

Shape::~Shape()
{
}

void Shape::_onDraw()
{
	// 形状是否显示
	if (!m_bDisplay) 
	{
		return;
	}
	// 设置线条和填充颜色
	setlinecolor(lineColor);
	setfillcolor(fillColor);

	// 根据形状的样式进行不同的绘制
	if (m_eStyle == Shape::STYLE::ROUND) 
	{
		roundShape();
	}
	else if (m_eStyle == Shape::STYLE::SOLID) 
	{
		solidShape();
	}
	else if (m_eStyle == Shape::STYLE::FILL) 
	{
		fillShape();
	}
}

inline COLORREF Shape::getFillColor() const
{
	return fillColor;
}

inline COLORREF Shape::getLineColor() const
{
	return lineColor;
}

void Shape::setFillColor(COLORREF color)
{
	fillColor = color;
}

void Shape::setLineColor(COLORREF color)
{
	lineColor = color;
}

void Shape::setStyle(STYLE style)
{
	m_eStyle = style;
}
