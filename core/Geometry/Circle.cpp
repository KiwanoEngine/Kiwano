#include "..\egeometry.h"
#include "..\enodes.h"

e2d::ECircle::ECircle()
	: m_pD2dCircle(nullptr)
{
}

e2d::ECircle::ECircle(EPoint center, float radius)
	: m_pD2dCircle(nullptr)
{
	this->_setCircle(center, radius);
}

e2d::ECircle::ECircle(ENode * node)
	: m_pD2dCircle(nullptr)
{
	float minSide = min(node->getRealWidth(), node->getRealHeight());
	this->_setCircle(
		EPoint(
			node->getRealWidth() / 2,
			node->getRealHeight() / 2
		),
		minSide / 2
	);
}

e2d::ECircle::~ECircle()
{
	SafeReleaseInterface(&m_pD2dCircle);
}

void e2d::ECircle::_setCircle(EPoint center, float radius)
{
	SafeReleaseInterface(&m_pD2dCircle);

	ERenderer::getID2D1Factory()->CreateEllipseGeometry(
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
