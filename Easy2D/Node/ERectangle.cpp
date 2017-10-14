#include "..\enodes.h"
#include "..\Win\winbase.h"


void e2d::ERectangle::_onRender()
{
	static D2D1_RECT_F rectangle = D2D1::RectF(
		m_Rect.left,
		m_Rect.top,
		m_Rect.right,
		m_Rect.bottom
	);
	GetRenderTarget()->FillRectangle(&rectangle, GetSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray)));
}

void e2d::ERectangle::_onTransfrom()
{
	
}
