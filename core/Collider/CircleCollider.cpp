#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::CircleCollider::CircleCollider()
	: _d2dCircle(nullptr)
{
}

e2d::CircleCollider::CircleCollider(Point center, double radius)
	: _d2dCircle(nullptr)
{
	this->setCircle(center, radius);
}

e2d::CircleCollider::CircleCollider(Node * node)
	: _d2dCircle(nullptr)
{
	double minSide = min(node->getRealWidth(), node->getRealHeight());
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
	SafeRelease(_d2dCircle);
}

void e2d::CircleCollider::setCircle(Point center, double radius)
{
	SafeRelease(_d2dCircle);

	Renderer::getID2D1Factory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				float(center.x), 
				float(center.y)),
			float(radius),
			float(radius)),
			&_d2dCircle
		);
}

void e2d::CircleCollider::_resize()
{
	if (_parentNode && _enable)
	{
		double minSide = min(_parentNode->getRealWidth(), _parentNode->getRealHeight());
		this->setCircle(
			Point(
				_parentNode->getRealWidth() / 2,
				_parentNode->getRealHeight() / 2
			),
			minSide / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::CircleCollider::getD2dGeometry() const
{
	return _d2dCircle;
}
