#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::EllipseCollider::EllipseCollider()
{
}

e2d::EllipseCollider::EllipseCollider(Point center, double radiusX, double radiusY)
{
	this->setEllipse(center, radiusX, radiusY);
}

e2d::EllipseCollider::EllipseCollider(Node * node)
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

e2d::EllipseCollider::~EllipseCollider()
{
}

void e2d::EllipseCollider::setEllipse(Point center, double radiusX, double radiusY)
{
	SafeRelease(_geometry);

	ID2D1EllipseGeometry* ellipse = nullptr;
	Renderer::getFactory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				float(center.x),
				float(center.y)),
			float(radiusX),
			float(radiusY)),
		&ellipse
	);
	_geometry = ellipse;
}

void e2d::EllipseCollider::_resize()
{
	if (_parentNode && _enabled)
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
