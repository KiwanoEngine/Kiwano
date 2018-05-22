#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderCircle::ColliderCircle()
	: _d2dCircle(nullptr)
{
}

e2d::ColliderCircle::ColliderCircle(Point center, double radius)
	: _d2dCircle(nullptr)
{
	this->setCircle(center, radius);
}

e2d::ColliderCircle::ColliderCircle(Node * node)
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

e2d::ColliderCircle::~ColliderCircle()
{
	SafeRelease(_d2dCircle);
}

void e2d::ColliderCircle::setCircle(Point center, double radius)
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

void e2d::ColliderCircle::_resize()
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

ID2D1EllipseGeometry * e2d::ColliderCircle::getD2dGeometry() const
{
	return _d2dCircle;
}
