#include "..\e2dcommon.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider(Node * parent)
	: _visible(true)
	, _color(Color::Blue, 0.6)
	, _parentNode(parent)
	, _geometry(nullptr)
	, _enabled(true)
	, _shape(Collider::Shape::None)
	, _notify(true)
{
}

e2d::Collider::~Collider()
{
	SafeRelease(_geometry);
	
}

e2d::Color e2d::Collider::getColor() const
{
	return _color;
}

e2d::Collider::Shape e2d::Collider::getShape() const
{
	return _shape;
}

e2d::Node * e2d::Collider::getNode() const
{
	return _parentNode;
}

ID2D1Geometry * e2d::Collider::getGeometry() const
{
	return _geometry;
}

void e2d::Collider::setShape(Shape shape)
{
	if (_shape == shape)
		return;

	_shape = shape;
	if (shape == Shape::None)
	{
		SafeRelease(_geometry);
		CollisionManager::getInstance()->__removeCollider(this);
	}
	else
	{
		this->recreate();
		CollisionManager::getInstance()->__addCollider(this);
	}
}

void e2d::Collider::setCollisionNotify(bool notify)
{
	_notify = notify;
}

void e2d::Collider::setEnabled(bool enabled)
{
	_enabled = enabled;
}

void e2d::Collider::setVisible(bool visible)
{
	_visible = visible;
}

void e2d::Collider::setColor(Color color)
{
	_color = color;
}

void e2d::Collider::render()
{
	if (_geometry && _enabled && _visible)
	{
		auto renderer = Renderer::getInstance();
		// 获取纯色画刷
		ID2D1SolidColorBrush * brush = renderer->getSolidColorBrush();
		// 设置画刷颜色和透明度
		brush->SetColor(_color.toD2DColorF());
		brush->SetOpacity(1.f);
		// 绘制几何碰撞体
		renderer->getRenderTarget()->DrawGeometry(_geometry, brush, 1.5f);
	}
}

e2d::Collider::Relation e2d::Collider::getRelationWith(Collider * collider) const
{
	if (_geometry && collider->_geometry)
	{
		if (_enabled && collider->_enabled)
		{
			D2D1_GEOMETRY_RELATION relation;
			_geometry->CompareWithGeometry(
				collider->_geometry,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return Relation(relation);
		}
	}
	return Relation::Unknown;
}

bool e2d::Collider::isEnabled() const
{
	return _enabled;
}

bool e2d::Collider::isVisible() const
{
	return _visible;
}

bool e2d::Collider::isCollisionNotify() const
{
	return _notify;
}

void e2d::Collider::recreate()
{
	if (!_enabled || _shape == Shape::None)
		return;

	SafeRelease(_geometry);

	switch (_shape)
	{
	case Shape::Rect:
	{
		ID2D1RectangleGeometry* rectangle = nullptr;
		Renderer::getFactory()->CreateRectangleGeometry(
			D2D1::RectF(
				0,
				0,
				float(_parentNode->getRealWidth()),
				float(_parentNode->getRealHeight())),
			&rectangle
		);
		_geometry = rectangle;
	}
	break;

	case Shape::Circle:
	{
		double minSide = std::min(_parentNode->getRealWidth(), _parentNode->getRealHeight());

		ID2D1EllipseGeometry* circle = nullptr;
		Renderer::getFactory()->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					float(_parentNode->getRealWidth() / 2),
					float(_parentNode->getRealHeight() / 2)
				),
				float(minSide / 2),
				float(minSide / 2)
			),
			&circle
		);
		_geometry = circle;
	}
	break;

	case Shape::Ellipse:
	{
		float halfWidth = float(_parentNode->getWidth() / 2),
			halfHeight = float(_parentNode->getHeight() / 2);

		ID2D1EllipseGeometry* ellipse = nullptr;
		Renderer::getFactory()->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					halfWidth,
					halfHeight),
				halfWidth,
				halfHeight),
			&ellipse
		);
		_geometry = ellipse;
	}
	break;
	}

	ID2D1TransformedGeometry * _transformed;
	Renderer::getFactory()->CreateTransformedGeometry(
		_geometry,
		_parentNode->_finalMatri,
		&_transformed
	);
	SafeRelease(_geometry);
	_geometry = _transformed;
}
