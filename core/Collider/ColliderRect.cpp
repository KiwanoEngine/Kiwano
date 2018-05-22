#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderRect::ColliderRect()
	: _d2dRectangle(nullptr)
{
}

e2d::ColliderRect::ColliderRect(double x, double y, double width, double height)
	: _d2dRectangle(nullptr)
{
	this->setRect(x, y, x + width, y + height);
}

e2d::ColliderRect::ColliderRect(Node * node)
	: _d2dRectangle(nullptr)
{
	this->setRect(0, 0, node->getRealWidth(), node->getRealHeight());
	this->setAutoResize(true);
}

e2d::ColliderRect::~ColliderRect()
{
	SafeRelease(_d2dRectangle);
}

void e2d::ColliderRect::setRect(double left, double top, double right, double bottom)
{
	SafeRelease(_d2dRectangle);

	Renderer::getID2D1Factory()->CreateRectangleGeometry(
		D2D1::RectF(
			float(left), 
			float(top), 
			float(right), 
			float(bottom)),
		&_d2dRectangle
	);
}

void e2d::ColliderRect::_resize()
{
	if (_parentNode && _enable)
	{
		this->setRect(0, 0, _parentNode->getRealWidth(), _parentNode->getRealHeight());
	}
}

ID2D1RectangleGeometry * e2d::ColliderRect::getD2dGeometry() const
{
	return _d2dRectangle;
}
