#include "..\e2dcollider.h"
#include "..\e2dnode.h"

e2d::ColliderEllipse::ColliderEllipse()
	: _pD2dEllipse(nullptr)
{
}

e2d::ColliderEllipse::ColliderEllipse(Point center, double radiusX, double radiusY)
	: _pD2dEllipse(nullptr)
{
	this->setEllipse(center, radiusX, radiusY);
}

e2d::ColliderEllipse::ColliderEllipse(Node * node)
	: _pD2dEllipse(nullptr)
{
	this->setEllipse(
		Point(
			node->getWidth() / 2,
			node->getHeight() / 2
		),
		node->getWidth() / 2,
		node->getHeight() / 2
	);
}

e2d::ColliderEllipse::~ColliderEllipse()
{
	SafeReleaseInterface(&_pD2dEllipse);
}

void e2d::ColliderEllipse::setEllipse(Point center, double radiusX, double radiusY)
{
	SafeReleaseInterface(&_pD2dEllipse);

	Renderer::getID2D1Factory()->CreateEllipseGeometry(
		D2D1::Ellipse(
			D2D1::Point2F(
				static_cast<float>(center.x),
				static_cast<float>(center.y)),
			static_cast<float>(radiusX),
			static_cast<float>(radiusY)),
		&_pD2dEllipse
	);
}

void e2d::ColliderEllipse::_resize()
{
	if (_pParentNode && _bEnable)
	{
		this->setEllipse(
			Point(
				_pParentNode->getWidth() / 2,
				_pParentNode->getHeight() / 2
			),
			_pParentNode->getWidth() / 2,
			_pParentNode->getHeight() / 2
		);
	}
}

ID2D1EllipseGeometry * e2d::ColliderEllipse::getD2dGeometry() const
{
	return _pD2dEllipse;
}
