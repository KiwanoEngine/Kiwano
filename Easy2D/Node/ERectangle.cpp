#include "..\enodes.h"
#include "..\Win\winbase.h"


e2d::ERectangle::ERectangle()
{
}

e2d::EColor::Enum e2d::ERectangle::getColor() const
{
	return m_Color;
}

void e2d::ERectangle::setColor(EColor::Enum color)
{
	m_Color = color;
}

void e2d::ERectangle::_onRender()
{
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, m_fWidth, m_fHeight);
	GetSolidColorBrush()->SetColor(D2D1::ColorF(m_Color, m_fDisplayOpacity));
	GetRenderTarget()->FillRectangle(&rectangle, GetSolidColorBrush());
}
