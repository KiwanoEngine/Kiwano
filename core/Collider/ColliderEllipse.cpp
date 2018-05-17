#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderEllipse::ColliderEllipse()
	: _d2dEllipse(nullptr)
{
}

e2d::ColliderEllipse::ColliderEllipse(Point center, double radiusX, double radiusY)
	: _d2dEllipse(nullptr)
{
	this->setEllipse(center, radiusX, radiusY);
}

e2d::ColliderEllipse::ColliderEllipse(Node * node)
	: _d2dEllipse(nullptr)
{
	this->setEllipse(
		Point(
			node->getWidth() / 2,
			node->getHeight() / 2
		),
		node->getWidth() / 2,
		node->getHeight() / 2
	);
	this->setAutoResize(true);
}

e2d::ColliderEllipse * e2d::ColliderEllipse::create()
{
	return Create<ColliderEllipse>();
}

e2d::ColliderEllipse * e2d::ColliderEllipse::create(Point center, double radiusX, double radiusY)
{
	return Create<ColliderEllipse>(center, radiusX, radiusY);
}

e2d::ColliderEllipse * e2d::ColliderEllipse::create(Node * node)
{
	return Create<ColliderEllipse>(node);
}

e2d::ColliderEllipse::~ColliderEllipse()
{
	SafeReleaseInterface(_d2dEllipse);
}

void e2d::ColliderEllipse::setEllipse(Point center, double radiusX, double radiusY)
{
	SafeReleaseInterface(_d2dEllipse);

	Renderer::getID2D1Factory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				float(center.x),
				float(center.y)),
			float(radiusX),
			float(radiusY)),
		&_d2dEllipse
	);
}

void e2d::ColliderEllipse::_resize()
{
	if (_parentNode && _enable)
	{
		this->setEllipse(
			Point(
				_parentNode->getWidth() / 2,
				_parentNode->getHeight() / 2
			),
			_parentNode->getWidth() / 2,
			_parentNode->getHeight() / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::ColliderEllipse::getD2dGeometry() const
{
	return _d2dEllipse;
}
