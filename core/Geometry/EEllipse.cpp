#include "..\egeometry.h"
#include "..\enodes.h"
#include "..\Win\winbase.h"

e2d::EEllipse::EEllipse()
	: m_pD2dEllipse(nullptr)
{
}

e2d::EEllipse::EEllipse(EPoint center, float radiusX, float radiusY)
	: m_pD2dEllipse(nullptr)
{
	this->_setEllipse(center, radiusX, radiusY);
}

e2d::EEllipse::EEllipse(ENode * node)
	: m_pD2dEllipse(nullptr)
{
	this->_setEllipse(
		EPoint(
			node->getWidth() / 2,
			node->getHeight() / 2
		),
		node->getWidth() / 2,
		node->getHeight() / 2
	);
}

e2d::EEllipse::~EEllipse()
{
	SafeReleaseInterface(&m_pD2dEllipse);
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
