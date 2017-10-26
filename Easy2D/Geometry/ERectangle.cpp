#include "..\egeometry.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"

e2d::ERectangle::ERectangle()
{
	this->_setRect(0, 0, 0, 0);
}

e2d::ERectangle::ERectangle(float x, float y, float width, float height)
{
	this->_setRect(x, y, x + width, y + height);
}

e2d::ERectangle::ERectangle(ENode * node)
{
	// ¼ÆËã×óÉÏ½Ç×ø±ê
	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(
		node->getPosX() - node->getRealWidth() * node->getAnchorX(),
		node->getPosY() - node->getRealHeight() * node->getAnchorY()
	);
	this->_setRect(
		upperLeftCorner.x,
		upperLeftCorner.y,
		upperLeftCorner.x + node->getRealWidth(),
		upperLeftCorner.y + node->getRealHeight()
	);
}

void e2d::ERectangle::_setRect(float left, float top, float right, float bottom)
{
	SafeReleaseInterface(&m_pD2dRectangle);

	GetFactory()->CreateRectangleGeometry(
		D2D1::RectF(left, top, right, bottom),
		&m_pD2dRectangle
	);
}

ID2D1RectangleGeometry * e2d::ERectangle::_getD2dGeometry() const
{
	return m_pD2dRectangle;
}
