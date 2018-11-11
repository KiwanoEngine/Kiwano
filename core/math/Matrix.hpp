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
#include "vector.hpp"
#include <d2d1.h>

namespace easy2d
{
	namespace math
	{
		class Matrix
		{
			float _11;
			float _12;
			float _21;
			float _22;
			float _31;
			float _32;

		public:
			Matrix()
				: _11(1.f)
				, _12(0.f)
				, _21(0.f)
				, _22(1.f)
				, _31(0.f)
				, _32(0.f)
			{
			}

			Matrix(
				float _11,
				float _12,
				float _21,
				float _22,
				float _31,
				float _32)
			{
				this->_11 = _11;
				this->_12 = _12;
				this->_21 = _21;
				this->_22 = _22;
				this->_31 = _31;
				this->_32 = _32;
			}

			inline Matrix operator*(const Matrix &matrix) const
			{
				return Matrix(
					_11 * matrix._11 + _12 * matrix._21,
					_11 * matrix._12 + _12 * matrix._22,
					_21 * matrix._11 + _22 * matrix._21,
					_21 * matrix._12 + _22 * matrix._22,
					_31 * matrix._11 + _32 * matrix._21 + matrix._31,
					_31 * matrix._12 + _32 * matrix._22 + matrix._32
				);
			}

			inline operator D2D1_MATRIX_3X2_F () const
			{
				return D2D1_MATRIX_3X2_F{
					_11, _12,
					_21, _22,
					_31, _32
				};
			}

			inline Matrix& Identity()
			{
				_11 = 1.f;
				_12 = 0.f;
				_21 = 0.f;
				_22 = 1.f;
				_31 = 0.f;
				_32 = 0.f;
				return *this;
			}

			inline Matrix& Translate(const Vector2& v)
			{
				*this = *this * Matrix::Translation(v);
				return *this;
			}

			inline Matrix& Translate(float x, float y)
			{
				*this = *this * Matrix::Translation(x, y);
				return *this;
			}

			inline Matrix& Scale(const Vector2& v, const Vector2& center)
			{
				*this = *this * Matrix::Scaling(v, center);
				return *this;
			}

			inline Matrix& Scale(float xscale, float yscale, const Vector2& center)
			{
				*this = *this * Matrix::Scaling(xscale, yscale, center);
				return *this;
			}

			inline Matrix& Rotate(float angle, const Vector2& center)
			{
				*this = *this * Matrix::Rotation(angle, center);
				return *this;
			}

			inline Matrix& Skew(float angle_x, float angle_y, const Vector2& center)
			{
				*this = *this * Matrix::Skewing(angle_x, angle_y, center);
				return *this;
			}

			inline float Determinant() const
			{
				return (_11 * _22) - (_12 * _21);
			}

			inline bool IsIdentity() const
			{
				return	_11 == 1.f && _12 == 0.f &&
						_21 == 0.f && _22 == 1.f &&
						_31 == 0.f && _32 == 0.f;
			}

			Vector2 Transform(const Vector2& v) const
			{
				return Vector2(
					v.x * _11 + v.y * _21 + _31,
					v.x * _12 + v.y * _22 + _32
				);
			}

			static Matrix Translation(const Vector2& v)
			{
				return Matrix(
					1.f, 0.f,
					0.f, 1.f,
					v.x, v.y
				);
			}

			static Matrix Translation(
				float x,
				float y)
			{
				return Translation(Vector2(x, y));
			}

			static Matrix Scaling(
				const Vector2& v,
				const Vector2& center = Vector2())
			{
				return Matrix(
					v.x, 0.f,
					0.f, v.y,
					center.x - v.x * center.x,
					center.y - v.y * center.y
				);
			}

			static Matrix Scaling(
				float x,
				float y,
				const Vector2& center = Vector2())
			{
				return Scaling(Vector2(x, y), center);
			}

			static Matrix Rotation(
				float angle,
				const Vector2& center = Vector2())
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

			static Matrix Skewing(
				float angle_x,
				float angle_y,
				const Vector2& center = Vector2())
			{
				float tx = math::Tan(angle_x);
				float ty = math::Tan(angle_y);
				return Matrix(
					1.f, tx,
					ty, 1.f,
					-center.y * tx, -center.x * ty
				);
			}
		};
	}
}
