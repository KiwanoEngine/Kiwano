#include "..\e2dutil.h"
#include "..\e2dimpl.h"
#include "..\e2dmodule.h"
#include "..\e2dmanager.h"
#include "..\e2dnode.h"

e2d::Collider::Collider(Node * parent)
	: visible_(true)
	, border_color_(Color::Blue, 0.6f)
	, parent_node_(parent)
	, geometry_(nullptr)
	, enabled_(true)
	, shape_(Collider::Shape::None)
	, notify_(true)
{
}

e2d::Collider::~Collider()
{
	SafeRelease(geometry_);
}

const e2d::Color& e2d::Collider::GetBorderColor() const
{
	return border_color_;
}

e2d::Collider::Shape e2d::Collider::GetShape() const
{
	return shape_;
}

e2d::Node * e2d::Collider::GetNode() const
{
	return parent_node_;
}

ID2D1Geometry * e2d::Collider::GetGeometry() const
{
	return geometry_;
}

void e2d::Collider::SetShape(Shape shape)
{
	if (shape_ == shape)
		return;

	shape_ = shape;
	if (shape == Shape::None)
	{
		SafeRelease(geometry_);
		CollisionManager::GetInstance()->RemoveCollider(this);
	}
	else
	{
		this->Recreate();
		CollisionManager::GetInstance()->AddCollider(this);
	}
}

void e2d::Collider::SetCollisionNotify(bool notify)
{
	notify_ = notify;
}

void e2d::Collider::SetEnabled(bool enabled)
{
	enabled_ = enabled;
}

void e2d::Collider::SetVisible(bool visible)
{
	visible_ = visible;
}

void e2d::Collider::SetBorderColor(const Color& color)
{
	border_color_ = color;
}

void e2d::Collider::Draw()
{
	if (geometry_ && enabled_ && visible_)
	{
		auto renderer = Renderer::GetInstance();
		// 获取纯色画刷
		ID2D1SolidColorBrush * brush = renderer->GetSolidBrush();
		// 设置画刷颜色和透明度
		brush->SetColor((D2D1_COLOR_F)border_color_);
		brush->SetOpacity(1.f);
		// 绘制几何碰撞体
		renderer->GetRenderTarget()->DrawGeometry(geometry_, brush, 1.5f);
	}
}

e2d::Collider::Relation e2d::Collider::GetRelationWith(Collider * collider) const
{
	if (geometry_ && collider->geometry_)
	{
		if (enabled_ && collider->enabled_)
		{
			D2D1_GEOMETRY_RELATION relation;
			geometry_->CompareWithGeometry(
				collider->geometry_,
				D2D1::Matrix3x2F::Identity(),
				&relation
			);

			return Relation(relation);
		}
	}
	return Relation::Unknown;
}

bool e2d::Collider::IsEnabled() const
{
	return enabled_;
}

bool e2d::Collider::IsVisible() const
{
	return visible_;
}

bool e2d::Collider::IsCollisionNotify() const
{
	return notify_;
}

void e2d::Collider::Recreate()
{
	if (!enabled_ || shape_ == Shape::None)
		return;

	SafeRelease(geometry_);
	auto factory = Renderer::GetFactory();

	switch (shape_)
	{
	case Shape::Rect:
	{
		ID2D1RectangleGeometry* rectangle = nullptr;
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, parent_node_->GetRealWidth(), parent_node_->GetRealHeight()),
			&rectangle
		);
		geometry_ = rectangle;
	}
	break;

	case Shape::Circle:
	{
		float minSide = std::min(parent_node_->GetRealWidth(), parent_node_->GetRealHeight());

		ID2D1EllipseGeometry* circle = nullptr;
		factory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					parent_node_->GetRealWidth() / 2,
					parent_node_->GetRealHeight() / 2
				),
				minSide / 2,
				minSide / 2
			),
			&circle
		);
		geometry_ = circle;
	}
	break;

	case Shape::Ellipse:
	{
		float halfWidth = parent_node_->GetWidth() / 2,
			halfHeight = parent_node_->GetHeight() / 2;

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
		geometry_ = ellipse;
	}
	break;
	}

	ID2D1TransformedGeometry * transformed;
	factory->CreateTransformedGeometry(
		geometry_,
		parent_node_->final_matrix_,
		&transformed
	);
	SafeRelease(geometry_);
	geometry_ = transformed;
}
