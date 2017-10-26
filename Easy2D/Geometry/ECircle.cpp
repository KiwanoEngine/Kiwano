#include "..\egeometry.h"
#include "..\Win\winbase.h"

e2d::ECircle::ECircle()
{
	this->_setCircle(EPoint(), 0);
}

e2d::ECircle::ECircle(EPoint center, float radius)
{
	this->_setCircle(center, radius);
}

void e2d::ECircle::_setCircle(EPoint center, float radius)
{
	SafeReleaseInterface(&m_pD2dCircle);

	GetFactory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				center.x, 
				center.y),
			radius,
			radius),
			&m_pD2dCircle
		);
}

ID2D1EllipseGeometry * e2d::ECircle::_getD2dGeometry() const
{
	return m_pD2dCircle;
}
