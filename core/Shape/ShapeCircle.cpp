#include "..\eshape.h"
#include "..\enodes.h"

e2d::ShapeCircle::ShapeCircle()
	: m_pD2dCircle(nullptr)
{
}

e2d::ShapeCircle::ShapeCircle(Point center, double radius)
	: m_pD2dCircle(nullptr)
{
	this->setCircle(center, radius);
}

e2d::ShapeCircle::ShapeCircle(Node * node)
	: m_pD2dCircle(nullptr)
{
	double minSide = min(node->getRealWidth(), node->getRealHeight());
	this->setCircle(
		Point(
			node->getRealWidth() / 2,
			node->getRealHeight() / 2
		),
		minSide / 2
	);
}

e2d::ShapeCircle::~ShapeCircle()
{
	SafeReleaseInterface(&m_pD2dCircle);
}

void e2d::ShapeCircle::setCircle(Point center, double radius)
{
	SafeReleaseInterface(&m_pD2dCircle);

	Renderer::getID2D1Factory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				static_cast<float>(center.x), 
				static_cast<float>(center.y)),
			static_cast<float>(radius),
			static_cast<float>(radius)),
			&m_pD2dCircle
		);
}

void e2d::ShapeCircle::_resize()
{
	if (m_pParentNode && m_bEnable)
	{
		double minSide = min(m_pParentNode->getRealWidth(), m_pParentNode->getRealHeight());
		this->setCircle(
			Point(
				m_pParentNode->getRealWidth() / 2,
				m_pParentNode->getRealHeight() / 2
			),
			minSide / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::ShapeCircle::getD2dGeometry() const
{
	return m_pD2dCircle;
}
