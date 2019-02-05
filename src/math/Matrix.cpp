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

#include "Matrix.h"

namespace easy2d
{
	namespace math
	{
		Matrix::Matrix()
			: _11(1.f), _12(0.f)
			, _21(0.f), _22(1.f)
			, _31(0.f), _32(0.f)
		{
		}

		Matrix::Matrix(float _11, float _12, float _21, float _22, float _31, float _32)
			: _11(_11), _12(_12), _21(_21), _22(_22), _31(_31), _32(_32)
		{
		}

		Matrix::Matrix(const float* p)
		{
			for (int i = 0; i < 6; i++)
				m[i] = p[i];
		}

		Matrix::Matrix(Matrix const& other)
			: _11(other._11), _12(other._12)
			, _21(other._21), _22(other._22)
			, _31(other._31), _32(other._32)
		{
		}

		void Matrix::Identity()
		{
			_11 = 1.f; _12 = 0.f;
			_21 = 0.f; _12 = 1.f;
			_31 = 0.f; _32 = 0.f;
		}

		Rect Matrix::Transform(const Rect & rect) const
		{
			Vec2 top_left = Transform(rect.GetLeftTop());
			Vec2 top_right = Transform(rect.GetRightTop());
			Vec2 bottom_left = Transform(rect.GetLeftBottom());
			Vec2 bottom_right = Transform(rect.GetRightBottom());

			float left = min(min(top_left.x, top_right.x), min(bottom_left.x, bottom_right.x));
			float right = max(max(top_left.x, top_right.x), max(bottom_left.x, bottom_right.x));
			float top = min(min(top_left.y, top_right.y), min(bottom_left.y, bottom_right.y));
			float bottom = max(max(top_left.y, top_right.y), max(bottom_left.y, bottom_right.y));

			return Rect{ left, top, (right - left), (bottom - top) };
		}

		Matrix Matrix::Translation(const Vec2& v)
		{
			return Matrix(
				1.f, 0.f,
				0.f, 1.f,
				v.x, v.y
			);
		}

		Matrix Matrix::Translation(
			float x,
			float y)
		{
			return Translation(Vec2(x, y));
		}

		Matrix Matrix::Scaling(
			const Vec2& v,
			const Vec2& center)
		{
			return Matrix(
				v.x, 0.f,
				0.f, v.y,
				center.x - v.x * center.x,
				center.y - v.y * center.y
			);
		}

		Matrix Matrix::Scaling(
			float x,
			float y,
			const Vec2& center)
		{
			return Scaling(Vec2(x, y), center);
		}

		Matrix Matrix::Rotation(
			float angle,
			const Vec2& center)
		{
			float s = math::Sin(angle);
			float c = math::Cos(angle);
			return Matrix(
				c, s,
				-s, c,
				center.x * (1 - c) + center.y * s,
				center.y * (1 - c) - center.x * s
			);
		}

		Matrix Matrix::Skewing(
			float angle_x,
			float angle_y,
			const Vec2& center)
		{
			float tx = math::Tan(angle_x);
			float ty = math::Tan(angle_y);
			return Matrix(
				1.f, -ty,
				-tx, 1.f,
				center.y * tx, center.x * ty
			);
		}

		Matrix Matrix::Invert(Matrix const& matrix)
		{
			float det = 1.f / matrix.Determinant();

			return Matrix(
				det * matrix._22,
				-det * matrix._12,
				-det * matrix._21,
				det * matrix._11,
				det * (matrix._21 * matrix._32 - matrix._22 * matrix._31),
				det * (matrix._12 * matrix._31 - matrix._11 * matrix._32)
			);
		}
	}
}
