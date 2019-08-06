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
#include "../base/BaseTypes.hpp"
#include "Matrix.hpp"

namespace easy2d
{
	namespace math
	{
		class Transform
		{
		public:
			Size size;				// ��С
			float rotation;			// ��ת
			math::Vector2 position;	// ����
			math::Vector2 scale;	// ����
			math::Vector2 skew;		// ���нǶ�
			math::Vector2 pivot;	// ֧��

		public:
			Transform()
				: position()
				, size()
				, rotation(0)
				, scale(1.f, 1.f)
				, skew(0.f, 0.f)
				, pivot(0.f, 0.f)
			{}

			inline Matrix ToMatrix() const
			{
				auto center = Vector2{ size.width * pivot.x, size.height * pivot.y };
				return Matrix{} * Matrix::Scaling(scale.x, scale.y, center)
					* Matrix::Skewing(skew.x, skew.y, center)
					* Matrix::Rotation(rotation, center)
					* Matrix::Translation(position - center);
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
	}
}
