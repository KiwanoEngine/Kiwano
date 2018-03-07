#include "..\eshape.h"
#include "..\enodes.h"

e2d::Ellipse::Ellipse()
	: m_pD2dEllipse(nullptr)
{
}

e2d::Ellipse::Ellipse(Point center, double radiusX, double radiusY)
	: m_pD2dEllipse(nullptr)
{
	this->_setEllipse(center, radiusX, radiusY);
}

e2d::Ellipse::Ellipse(Node * node)
	: m_pD2dEllipse(nullptr)
{
	this->_setEllipse(
		Point(
			node->getWidth() / 2,
			node->getHeight() / 2
		),
		node->getWidth() / 2,
		node->getHeight() / 2
	);
}

e2d::Ellipse::~Ellipse()
{
	SafeReleaseInterface(&m_pD2dEllipse);
}

void e2d::Ellipse::_setEllipse(Point center, double radiusX, double radiusY)
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

void e2d::Ellipse::_resize()
{
	if (m_pParentNode && m_bEnable)
	{
		this->_setEllipse(
			Point(
				m_pParentNode->getWidth() / 2,
				m_pParentNode->getHeight() / 2
			),
			m_pParentNode->getWidth() / 2,
			m_pParentNode->getHeight() / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::Ellipse::_getD2dGeometry() const
{
	return m_pD2dEllipse;
}
