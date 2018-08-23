#include "..\e2dcommon.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider(Node * parent)
	: _visible(true)
	, _color(Color::Blue, 0.6f)
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

e2d::Color e2d::Collider::color() const
{
	return _color;
}

e2d::Collider::Shape e2d::Collider::shape() const
{
	return _shape;
}

e2d::Node * e2d::Collider::node() const
{
	return _parentNode;
}

ID2D1Geometry * e2d::Collider::geometry() const
{
	return _geometry;
}

e2d::Collider& e2d::Collider::shape(Shape shape)
{
	if (_shape == shape)
		return *this;

	_shape = shape;
	if (shape == Shape::None)
	{
		SafeRelease(_geometry);
		CollisionManager::instance()->__removeCollider(this);
	}
	else
	{
		this->recreate();
		CollisionManager::instance()->__addCollider(this);
	}
	return *this;
}

e2d::Collider& e2d::Collider::notify(bool notify)
{
	_notify = notify;
	return *this;
}

e2d::Collider& e2d::Collider::enabled(bool enabled)
{
	_enabled = enabled;
	return *this;
}

e2d::Collider& e2d::Collider::visible(bool visible)
{
	_visible = visible;
	return *this;
}

e2d::Collider& e2d::Collider::color(Color color)
{
	_color = color;
	return *this;
}

void e2d::Collider::render()
{
	if (_geometry && _enabled && _visible)
	{
		auto renderer = Game::instance()->renderer();
		// 获取纯色画刷
		ID2D1SolidColorBrush * brush = renderer->solidBrush();
		// 设置画刷颜色和透明度
		brush->SetColor((D2D1_COLOR_F)_color);
		brush->SetOpacity(1.f);
		// 绘制几何碰撞体
		renderer->renderTarget()->DrawGeometry(_geometry, brush, 1.5f);
	}
}

e2d::Collider::Relation e2d::Collider::relationWith(Collider * collider) const
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

bool e2d::Collider::enabled() const
{
	return _enabled;
}

bool e2d::Collider::visible() const
{
	return _visible;
}

bool e2d::Collider::notify() const
{
	return _notify;
}

void e2d::Collider::recreate()
{
	if (!_enabled || _shape == Shape::None)
		return;

	SafeRelease(_geometry);
	auto factory = Game::instance()->renderer()->factory();

	switch (_shape)
	{
	case Shape::Rect:
	{
		ID2D1RectangleGeometry* rectangle = nullptr;
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _parentNode->realWidth(), _parentNode->realHeight()),
			&rectangle
		);
		_geometry = rectangle;
	}
	break;

	case Shape::Circle:
	{
		float minSide = std::min(_parentNode->realWidth(), _parentNode->realHeight());

		ID2D1EllipseGeometry* circle = nullptr;
		factory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_parentNode->realWidth() / 2,
					_parentNode->realHeight() / 2
				),
				minSide / 2,
				minSide / 2
			),
			&circle
		);
		_geometry = circle;
	}
	break;

	case Shape::Ellipse:
	{
		float halfWidth = _parentNode->width() / 2,
			halfHeight = _parentNode->height() / 2;

		ID2D1EllipseGeometry* ellipse = nullptr;
		factory->CreateEllipseGeometry(
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
	factory->CreateTransformedGeometry(
		_geometry,
		_parentNode->_finalMatri,
		&_transformed
	);
	SafeRelease(_geometry);
	_geometry = _transformed;
}
