#include "..\easy2d.h"
#include "..\EasyX\easyx.h"


Text::Text() : 
	m_sText(_T("")),
	m_color(Color::white),
	m_pFontStyle(FontStyle::getDefault())
{
	m_pFontStyle->retain();		// 字体引用计数加一
}

Text::Text(TString text, COLORREF color, FontStyle * font) : 
	m_color(color),
	m_pFontStyle(font)
{
	setText(text);
	m_pFontStyle->retain();		// 字体引用计数加一
}

Text::Text(int x, int y, TString text, COLORREF color, FontStyle * font) :
	m_color(color),
	m_pFontStyle(font)
{
	setText(text);
	setPos(x, y);
	m_pFontStyle->retain();		// 字体引用计数加一
}

Text::~Text()
{
	SafeRelease(m_pFontStyle);	// 字体引用计数减一
}

void Text::_onDraw()
{
	// 若 display 属性为 false，不绘制这个文本
	if (!m_bDisplay) 
	{
		return;
	}
	// 设置字体
	settextstyle(&m_pFontStyle->m_font);
	// 设置文本颜色
	settextcolor(m_color);
	// 输出文字
	outtextxy(getX(), getY(), m_sText.c_str());
}

COLORREF Text::getColor() const
{
	return m_color;
}

TString Text::getText() const
{
	return m_sText;
}

FontStyle * Text::getFontStyle()
{
	return m_pFontStyle;
}

bool Text::isEmpty() const
{
	return m_sText.empty();	// 文本是否为空
}

void Text::setText(TString text)
{
	m_sText = text;
	// 先设置字体，然后获取该文本在该字体下的宽度和高度
	settextstyle(&m_pFontStyle->m_font);
	setSize(textwidth(getText().c_str()), textheight(getText().c_str()));
}

void Text::setColor(COLORREF color)
{
	m_color = color;
}

void Text::setFontStyle(FontStyle * style)
{
	SafeRelease(m_pFontStyle);	// 原字体引用计数减一
	m_pFontStyle = style;		// 修改字体
	m_pFontStyle->retain();		// 现字体引用计数加一
	// 先设置字体，然后获取该文本在该字体下的宽度和高度
	settextstyle(&m_pFontStyle->m_font);
	setSize(textwidth(getText().c_str()), textheight(getText().c_str()));
}
