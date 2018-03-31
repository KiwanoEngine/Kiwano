#include "..\eshape.h"
#include "..\enodes.h"

e2d::ShapeRectangle::ShapeRectangle()
	: m_pD2dRectangle(nullptr)
{
}

e2d::ShapeRectangle::ShapeRectangle(double x, double y, double width, double height)
	: m_pD2dRectangle(nullptr)
{
	this->setRect(x, y, x + width, y + height);
}

e2d::ShapeRectangle::ShapeRectangle(Node * node)
	: m_pD2dRectangle(nullptr)
{
	this->setRect(
		0,
		0,
		node->getRealWidth(),
		node->getRealHeight()
	);
}

e2d::ShapeRectangle::~ShapeRectangle()
{
	SafeReleaseInterface(&m_pD2dRectangle);
}

void e2d::ShapeRectangle::setRect(double left, double top, double right, double bottom)
{
	SafeReleaseInterface(&m_pD2dRectangle);

	Renderer::getID2D1Factory()->CreateRectangleGeometry(
		D2D1::RectF(
			static_cast<float>(left), 
			static_cast<float>(top), 
			static_cast<float>(right), 
			static_cast<float>(bottom)),
		&m_pD2dRectangle
	);
}

void e2d::ShapeRectangle::_resize()
{
	if (m_pParentNode && m_bEnable)
	{
		this->setRect(
			0,
			0,
			m_pParentNode->getRealWidth(),
			m_pParentNode->getRealHeight()
		);
	}
}

ID2D1RectangleGeometry * e2d::ShapeRectangle::getD2dGeometry() const
{
	return m_pD2dRectangle;
}
