// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "..\e2dutil.h"


easy2d::Transform::Transform()
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

easy2d::Transform::operator D2D1::Matrix3x2F() const
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

bool easy2d::Transform::operator==(const Transform & other) const
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
