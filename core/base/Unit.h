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

#pragma once
#include "base.hpp"
#include "Transform.hpp"

namespace easy2d
{
	class Unit
		: public RefCounter
		, public Object
	{
	public:
		Unit();

		virtual ~Unit();

		// 获取坐标
		virtual const Point& GetPosition() const { return transform_.position; }

		// 获取横向缩放比例
		virtual float GetScaleX() const { return transform_.scale.x; }

		// 获取纵向缩放比例
		virtual float GetScaleY() const { return transform_.scale.y; }

		// 获取横向错切角度
		virtual float GetSkewX() const { return transform_.skew.x; }

		// 获取纵向错切角度
		virtual float GetSkewY() const { return transform_.skew.y; }

		// 获取旋转角度
		virtual float GetRotation() const { return transform_.rotation; }

		Transform const& GetTransform() const { return transform_; }

		// 设置横坐标
		void SetPositionX(
			float x
		);

		// 设置纵坐标
		void SetPositionY(
			float y
		);

		// 设置坐标
		void SetPosition(
			const Point & point
		);

		// 设置坐标
		virtual void SetPosition(
			float x,
			float y
		);

		// 移动
		void Move(
			float x,
			float y
		);

		// 移动
		void Move(
			const Point & vector
		);

		// 设置横向缩放比例
		// 默认为 1.0
		void SetScaleX(
			float scale_x
		);

		// 设置纵向缩放比例
		// 默认为 1.0
		void SetScaleY(
			float scale_y
		);

		// 设置缩放比例
		// 默认为 (1.0, 1.0)
		virtual void SetScale(
			float scale_x,
			float scale_y
		);

		// 设置缩放比例
		// 默认为 1.0
		void SetScale(
			float scale
		);

		// 设置横向错切角度
		// 默认为 0
		void SetSkewX(
			float skew_x
		);

		// 设置纵向错切角度
		// 默认为 0
		void SetSkewY(
			float skew_y
		);

		// 设置错切角度
		// 默认为 (0, 0)
		virtual void SetSkew(
			float skew_x,
			float skew_y
		);

		// 设置旋转角度
		// 默认为 0
		virtual void SetRotation(
			float rotation
		);

		virtual void SetTransform(
			Transform const& transform
		);

		// 获取二维变换矩阵
		virtual math::Matrix const& GetTransformMatrix();

	protected:
		virtual void UpdateMatrix();

	protected:
		bool			dirty_transform_;
		Transform		transform_;
		math::Matrix	matrix_cached_;
	};
}
