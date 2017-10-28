#include "..\egeometry.h"
#include "..\Win\winbase.h"

e2d::EEllipse::EEllipse()
	: m_pD2dEllipse(nullptr)
{
	this->_setEllipse(EPoint(), 0, 0);
}

e2d::EEllipse::EEllipse(EPoint center, float radiusX, float radiusY)
	: EEllipse()
{
	this->_setEllipse(center, radiusX, radiusY);
}

void e2d::EEllipse::_setEllipse(EPoint center, float radiusX, float radiusY)
{
	SafeReleaseInterface(&m_pD2dEllipse);

	GetFactory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				center.x,
				center.y),
			radiusX,
			radiusY),
		&m_pD2dEllipse
	);
}

ID2D1EllipseGeometry * e2d::EEllipse::_getD2dGeometry() const
{
	return m_pD2dEllipse;
}
