#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderRect::ColliderRect()
	: _pD2dRectangle(nullptr)
{
}

e2d::ColliderRect::ColliderRect(double x, double y, double width, double height)
	: _pD2dRectangle(nullptr)
{
	this->setRect(x, y, x + width, y + height);
}

e2d::ColliderRect::ColliderRect(Node * node)
	: _pD2dRectangle(nullptr)
{
	this->setRect(0, 0, node->getRealWidth(), node->getRealHeight());
}

e2d::ColliderRect::~ColliderRect()
{
	SafeReleaseInterface(&_pD2dRectangle);
}

void e2d::ColliderRect::setRect(double left, double top, double right, double bottom)
{
	SafeReleaseInterface(&_pD2dRectangle);

	Renderer::getID2D1Factory()->CreateRectangleGeometry(
		D2D1::RectF(
			static_cast<float>(left), 
			static_cast<float>(top), 
			static_cast<float>(right), 
			static_cast<float>(bottom)),
		&_pD2dRectangle
	);
}

void e2d::ColliderRect::_resize()
{
	if (_pParentNode && _bEnable)
	{
		this->setRect( 0, 0, _pParentNode->getRealWidth(), _pParentNode->getRealHeight());
	}
}

ID2D1RectangleGeometry * e2d::ColliderRect::getD2dGeometry() const
{
	return _pD2dRectangle;
}
