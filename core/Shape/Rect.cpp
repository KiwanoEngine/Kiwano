#include "..\eshape.h"
#include "..\enodes.h"

e2d::Rect::Rect()
	: m_pD2dRectangle(nullptr)
{
}

e2d::Rect::Rect(float x, float y, float width, float height)
	: m_pD2dRectangle(nullptr)
{
	this->_setRect(x, y, x + width, y + height);
}

e2d::Rect::Rect(Node * node)
	: m_pD2dRectangle(nullptr)
{
	this->_setRect(
		0,
		0,
		node->getRealWidth(),
		node->getRealHeight()
	);
}

e2d::Rect::~Rect()
{
	SafeReleaseInterface(&m_pD2dRectangle);
}

void e2d::Rect::_setRect(float left, float top, float right, float bottom)
{
	SafeReleaseInterface(&m_pD2dRectangle);

	Renderer::getID2D1Factory()->CreateRectangleGeometry(
		D2D1::RectF(left, top, right, bottom),
		&m_pD2dRectangle
	);
}

ID2D1RectangleGeometry * e2d::Rect::_getD2dGeometry() const
{
	return m_pD2dRectangle;
}
