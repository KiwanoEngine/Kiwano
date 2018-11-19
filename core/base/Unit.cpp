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

#include "Unit.h"

namespace easy2d
{
	Unit::Unit()
		: transform_()
		, dirty_transform_(false)
	{
	}

	Unit::~Unit()
	{
	}

	math::Matrix const & Unit::GetTransformMatrix()
	{
		if (dirty_transform_)
		{
			UpdateMatrix();
			dirty_transform_ = false;
		}
		return matrix_cached_;
	}

	void Unit::SetPositionX(float x)
	{
		this->SetPosition(x, transform_.position.y);
	}

	void Unit::SetPositionY(float y)
	{
		this->SetPosition(transform_.position.x, y);
	}

	void Unit::SetPosition(const Point & p)
	{
		this->SetPosition(p.x, p.y);
	}

	void Unit::SetPosition(float x, float y)
	{
		if (transform_.position.x == x && transform_.position.y == y)
			return;

		transform_.position.x = x;
		transform_.position.y = y;
		dirty_transform_ = true;
	}

	void Unit::MoveBy(float x, float y)
	{
		this->SetPosition(transform_.position.x + x, transform_.position.y + y);
	}

	void Unit::MoveBy(const Point & v)
	{
		this->MoveBy(v.x, v.y);
	}

	void Unit::SetScaleX(float scale_x)
	{
		this->SetScale(scale_x, transform_.scale.y);
	}

	void Unit::SetScaleY(float scale_y)
	{
		this->SetScale(transform_.scale.x, scale_y);
	}

	void Unit::SetScale(float scale)
	{
		this->SetScale(scale, scale);
	}

	void Unit::SetScale(float scale_x, float scale_y)
	{
		if (transform_.scale.x == scale_x && transform_.scale.y == scale_y)
			return;

		transform_.scale.x = scale_x;
		transform_.scale.y = scale_y;
		dirty_transform_ = true;
	}

	void Unit::SetSkewX(float skew_x)
	{
		this->SetSkew(skew_x, transform_.skew.y);
	}

	void Unit::SetSkewY(float skew_y)
	{
		this->SetSkew(transform_.skew.x, skew_y);
	}

	void Unit::SetSkew(float skew_x, float skew_y)
	{
		if (transform_.skew.x == skew_x && transform_.skew.y == skew_y)
			return;

		transform_.skew.x = skew_x;
		transform_.skew.y = skew_y;
		dirty_transform_ = true;
	}

	void Unit::SetRotation(float angle)
	{
		if (transform_.rotation == angle)
			return;

		transform_.rotation = angle;
		dirty_transform_ = true;
	}

	void Unit::SetTransform(Transform const& transform)
	{
		transform_ = transform;
	}

	void Unit::UpdateMatrix()
	{
		Point center;
		matrix_cached_ = math::Matrix::Scaling(transform_.scale, center)
			* math::Matrix::Skewing(transform_.skew.x, transform_.skew.y, center)
			* math::Matrix::Rotation(transform_.rotation, center)
			* math::Matrix::Translation(transform_.position - center);
	}

}
