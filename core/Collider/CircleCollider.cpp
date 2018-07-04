#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::CircleCollider::CircleCollider()
{
}

e2d::CircleCollider::CircleCollider(Point center, double radius)
{
	this->setCircle(center, radius);
}

e2d::CircleCollider::CircleCollider(Node * node)
{
	double minSide = std::min(node->getRealWidth(), node->getRealHeight());
	this->setCircle(
		Point(
			node->getRealWidth() / 2,
			node->getRealHeight() / 2
		),
		minSide / 2
	);
	this->setAutoResize(true);
}

e2d::CircleCollider::~CircleCollider()
{
}

void e2d::CircleCollider::setCircle(Point center, double radius)
{
	SafeRelease(_geometry);

	ID2D1EllipseGeometry* circle = nullptr;
	Renderer::getFactory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				float(center.x), 
				float(center.y)
			),
			float(radius),
			float(radius)
		),
		&circle
	);

	_geometry = circle;
}

void e2d::CircleCollider::_resize()
{
	if (_parentNode && _enabled)
	{
		double minSide = std::min(_parentNode->getRealWidth(), _parentNode->getRealHeight());
		this->setCircle(
			Point(
				_parentNode->getRealWidth() / 2,
				_parentNode->getRealHeight() / 2
			),
			minSide / 2
		);
	}
}
