#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::RectCollider::RectCollider()
{
}

e2d::RectCollider::RectCollider(double x, double y, double width, double height)
{
	this->setRect(x, y, x + width, y + height);
}

e2d::RectCollider::RectCollider(Node * node)
{
	this->setRect(0, 0, node->getRealWidth(), node->getRealHeight());
	this->setAutoResize(true);
}

e2d::RectCollider::~RectCollider()
{
}

void e2d::RectCollider::setRect(double left, double top, double right, double bottom)
{
	SafeRelease(_geometry);

	ID2D1RectangleGeometry* rectangle = nullptr;
	Renderer::getFactory()->CreateRectangleGeometry(
		D2D1::RectF(
			float(left), 
			float(top), 
			float(right), 
			float(bottom)),
		&rectangle
	);

	_geometry = rectangle;
}

void e2d::RectCollider::_resize()
{
	if (_parentNode && _enabled)
	{
		this->setRect(0, 0, _parentNode->getRealWidth(), _parentNode->getRealHeight());
	}
}
