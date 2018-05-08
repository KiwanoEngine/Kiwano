#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderCircle::ColliderCircle()
	: _pD2dCircle(nullptr)
{
}

e2d::ColliderCircle::ColliderCircle(Point center, double radius)
	: _pD2dCircle(nullptr)
{
	this->setCircle(center, radius);
}

e2d::ColliderCircle::ColliderCircle(Node * node)
	: _pD2dCircle(nullptr)
{
	double minSide = min(node->getRealWidth(), node->getRealHeight());
	this->setCircle(
		Point(
			node->getRealWidth() / 2,
			node->getRealHeight() / 2
		),
		minSide / 2
	);
}

e2d::ColliderCircle::~ColliderCircle()
{
	SafeReleaseInterface(&_pD2dCircle);
}

void e2d::ColliderCircle::setCircle(Point center, double radius)
{
	SafeReleaseInterface(&_pD2dCircle);

	Renderer::getID2D1Factory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				static_cast<float>(center.x), 
				static_cast<float>(center.y)),
			static_cast<float>(radius),
			static_cast<float>(radius)),
			&_pD2dCircle
		);
}

void e2d::ColliderCircle::_resize()
{
	if (_pParentNode && _bEnable)
	{
		double minSide = min(_pParentNode->getRealWidth(), _pParentNode->getRealHeight());
		this->setCircle(
			Point(
				_pParentNode->getRealWidth() / 2,
				_pParentNode->getRealHeight() / 2
			),
			minSide / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::ColliderCircle::getD2dGeometry() const
{
	return _pD2dCircle;
}
