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
#include "Size.hpp"
#include "Point.hpp"
#include "../math/Matrix.hpp"
#include <d2d1.h>

namespace easy2d
{
	class Transform
	{
	public:
		Size size;			// 大小
		float rotation;		// 旋转
		Point position;		// 坐标
		Point scale;		// 缩放
		Point skew;			// 错切角度
		Point pivot;		// 支点

	public:
		Transform()
			: position()
			, size()
			, rotation(0)
			, scale(1.f, 1.f)
			, skew(0.f, 0.f)
			, pivot(0.f, 0.f)
		{}

		inline math::Matrix ToMatrix() const
		{
			math::Vector2 center{ size.width * pivot.x, size.height * pivot.y };
			return math::Matrix::Scaling(scale.x, scale.y, center)
				* math::Matrix::Skewing(skew.x, skew.y, center)
				* math::Matrix::Rotation(rotation, center)
				* math::Matrix::Translation(position.x - center.x, position.y - center.y);
		}

		bool operator== (const Transform& other) const
		{
			return position == other.position &&
				size == other.size &&
				scale == other.scale &&
				skew == other.skew &&
				rotation == other.rotation &&
				pivot == other.pivot;
		}
	};

	inline D2D1_MATRIX_3X2_F ConvertToD2DMatrix(math::Matrix const& matrix)
	{
		return D2D1_MATRIX_3X2_F{
				matrix.m[0], matrix.m[1],
				matrix.m[2], matrix.m[3],
				matrix.m[4], matrix.m[5]
		};
	}
}
