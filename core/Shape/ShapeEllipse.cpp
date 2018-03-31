#include "..\eshape.h"
#include "..\enodes.h"

e2d::ShapeEllipse::ShapeEllipse()
	: m_pD2dEllipse(nullptr)
{
}

e2d::ShapeEllipse::ShapeEllipse(Point center, double radiusX, double radiusY)
	: m_pD2dEllipse(nullptr)
{
	this->setEllipse(center, radiusX, radiusY);
}

e2d::ShapeEllipse::ShapeEllipse(Node * node)
	: m_pD2dEllipse(nullptr)
{
	this->setEllipse(
		Point(
			node->getWidth() / 2,
			node->getHeight() / 2
		),
		node->getWidth() / 2,
		node->getHeight() / 2
	);
}

e2d::ShapeEllipse::~ShapeEllipse()
{
	SafeReleaseInterface(&m_pD2dEllipse);
}

void e2d::ShapeEllipse::setEllipse(Point center, double radiusX, double radiusY)
{
	SafeReleaseInterface(&m_pD2dEllipse);

	Renderer::getID2D1Factory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				static_cast<float>(center.x),
				static_cast<float>(center.y)),
			static_cast<float>(radiusX),
			static_cast<float>(radiusY)),
		&m_pD2dEllipse
	);
}

void e2d::ShapeEllipse::_resize()
{
	if (m_pParentNode && m_bEnable)
	{
		this->setEllipse(
			Point(
				m_pParentNode->getWidth() / 2,
				m_pParentNode->getHeight() / 2
			),
			m_pParentNode->getWidth() / 2,
			m_pParentNode->getHeight() / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::ShapeEllipse::getD2dGeometry() const
{
	return m_pD2dEllipse;
}
