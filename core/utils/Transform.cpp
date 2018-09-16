#include "..\e2dutil.h"


e2d::Transform::Transform()
	: position()
	, size()
	, scale_x(1.f)
	, scale_y(1.f)
	, rotation(0)
	, skew_x(0)
	, skew_y(0)
	, pivot_x(0)
	, pivot_y(0)
{
}

e2d::Transform::operator D2D1::Matrix3x2F() const
{
	auto pivot = D2D1::Point2F(size.width * pivot_x, size.height * pivot_y);
	auto matrix = D2D1::Matrix3x2F::Scale(
		scale_x,
		scale_y,
		pivot
	) * D2D1::Matrix3x2F::Skew(
		skew_x,
		skew_y,
		pivot
	) * D2D1::Matrix3x2F::Rotation(
		rotation,
		pivot
	) * D2D1::Matrix3x2F::Translation(
		position.x - pivot.x,
		position.y - pivot.y
	);
	return std::move(matrix);
}

bool e2d::Transform::operator==(const Transform & other) const
{
	return position == other.position &&
		size == other.size &&
		scale_x == other.scale_x &&
		scale_y == other.scale_y &&
		skew_x == other.skew_x &&
		skew_y == other.skew_y &&
		rotation == other.rotation &&
		pivot_x == other.pivot_x &&
		pivot_y == other.pivot_y;
}
