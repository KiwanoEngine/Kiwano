#include "..\e2dcollider.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider(Node * parent)
	: _visible(true)
	, _color(Color::Red, 0.7)
	, _parentNode(parent)
	, _transformed(nullptr)
	, _geometry(nullptr)
	, _enabled(true)
	, _type(Collider::Type::None)
{
	_type = Game::getInstance()->getConfig()->getDefaultColliderType();
}

e2d::Collider::~Collider()
{
	SafeRelease(_transformed);
	SafeRelease(_geometry);
}

e2d::Color e2d::Collider::getColor() const
{
	return _color;
}

ID2D1Geometry * e2d::Collider::getGeometry() const
{
	return _geometry;
}

ID2D1TransformedGeometry * e2d::Collider::getTransformedGeometry() const
{
	return _transformed;
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

void e2d::Collider::_render()
{
	if (_transformed && _enabled)
	{
		auto renderer = Renderer::getInstance();
		// 获取纯色画刷
		ID2D1SolidColorBrush * brush = renderer->getSolidColorBrush();
		// 设置画刷颜色和透明度
		brush->SetColor(_color.toD2DColorF());
		brush->SetOpacity(1.f);
		// 绘制几何碰撞体
		renderer->getRenderTarget()->DrawGeometry(_transformed, brush);
	}
}

e2d::Collider::Relation e2d::Collider::getRelationWith(Collider * pCollider) const
{
	if (_transformed && pCollider->_transformed)
	{
		if (_enabled && pCollider->_enabled)
		{
			D2D1_GEOMETRY_RELATION relation;

			_transformed->CompareWithGeometry(
				pCollider->_transformed,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return Relation(relation);
		}
	}
	return Relation::Unknown;
}

void e2d::Collider::_recreate(Collider::Type type)
{
	_type = type;

	SafeRelease(_geometry);
	SafeRelease(_transformed);

	switch (type)
	{
	case Type::Rect:
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

	case Type::Circle:
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

	case Type::Ellipse:
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

	default:
		break;
	}
}

void e2d::Collider::_transform()
{
	if (_enabled && _type != Type::None)
	{
		// 重新生成碰撞体
		_recreate(_type);
		// 二维变换
		Renderer::getFactory()->CreateTransformedGeometry(
			_geometry,
			_parentNode->_finalMatri,
			&_transformed
		);
	}
}
