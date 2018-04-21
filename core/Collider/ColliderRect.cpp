#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderRect::ColliderRect()
	: m_pD2dRectangle(nullptr)
{
}

e2d::ColliderRect::ColliderRect(double x, double y, double width, double height)
	: m_pD2dRectangle(nullptr)
{
	this->setRect(x, y, x + width, y + height);
}

e2d::ColliderRect::ColliderRect(Node * node)
	: m_pD2dRectangle(nullptr)
{
	this->setRect(0, 0, node->getRealWidth(), node->getRealHeight());
}

e2d::ColliderRect::~ColliderRect()
{
	SafeReleaseInterface(&m_pD2dRectangle);
}

void e2d::ColliderRect::setRect(double left, double top, double right, double bottom)
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

void e2d::ColliderRect::_resize()
{
	if (m_pParentNode && m_bEnable)
	{
		this->setRect( 0, 0, m_pParentNode->getRealWidth(), m_pParentNode->getRealHeight());
	}
}

ID2D1RectangleGeometry * e2d::ColliderRect::getD2dGeometry() const
{
	return m_pD2dRectangle;
}
