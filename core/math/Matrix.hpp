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

namespace easy2d
{
	namespace math
	{
		class Matrix;

		template <typename L, typename R>
		struct MatrixMultiply;

		inline MatrixMultiply<Matrix, Matrix>
		operator *(Matrix const& lhs, Matrix const& rhs);

		template <typename L, typename R>
		inline MatrixMultiply<MatrixMultiply<L, R>, Matrix>
		operator *(MatrixMultiply<L, R> const& lhs, Matrix const& rhs);


		class Matrix
		{
		public:
			float m[6];  // m[3][2]

		public:
			Matrix()
			{
				m[0] = 1.f; m[1] = 0.f;
				m[2] = 0.f; m[3] = 1.f;
				m[4] = 0.f; m[5] = 0.f;
			}

			Matrix(float val[6])
			{
				m[0] = val[0]; m[1] = val[1];
				m[2] = val[2]; m[3] = val[3];
				m[4] = val[4]; m[5] = val[5];
			}

			Matrix(float _11, float _12, float _21, float _22, float _31, float _32)
			{
				m[0] = _11; m[1] = _12;
				m[2] = _21; m[3] = _22;
				m[4] = _31; m[5] = _32;
			}

			Matrix(Matrix const& other)
			{
				m[0] = other.m[0]; m[1] = other.m[1];
				m[2] = other.m[2]; m[3] = other.m[3];
				m[4] = other.m[4]; m[5] = other.m[5];
			}

			template <typename T>
			Matrix(T const& other)
			{
				m[0] = other[0]; m[1] = other[1];
				m[2] = other[2]; m[3] = other[3];
				m[4] = other[4]; m[5] = other[5];
			}

			inline float operator [](unsigned int index) const { return m[index]; }

			template <typename T>
			inline Matrix& operator =(T const& other)
			{
				m[0] = other[0]; m[1] = other[1];
				m[2] = other[2]; m[3] = other[3];
				m[4] = other[4]; m[5] = other[5];
				return *this;
			}

			inline Matrix& Identity()
			{
				m[0] = 1.f; m[1] = 0.f;
				m[2] = 0.f; m[3] = 1.f;
				m[4] = 0.f; m[5] = 0.f;
				return *this;
			}

			inline float Determinant() const
			{
				return (m[0] * m[3]) - (m[1] * m[2]);
			}

			inline bool IsIdentity() const
			{
				return	m[0] == 1.f && m[1] == 0.f &&
						m[2] == 0.f && m[3] == 1.f &&
						m[4] == 0.f && m[5] == 0.f;
			}

			Vector2 Transform(const Vector2& v) const
			{
				return Vector2(
					v.x * m[0] + v.y * m[2] + m[4],
					v.x * m[1] + v.y * m[3] + m[5]
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
					1.f, -ty,
					-tx, 1.f,
					center.y * tx, center.x * ty
				);
			}
		};


		template <typename L, typename R>
		struct MatrixMultiply
		{
			L const& lhs;
			R const& rhs;

			MatrixMultiply(L const& lhs, R const& rhs)
				: lhs(lhs)
				, rhs(rhs)
			{}

			inline float operator [](unsigned int index) const
			{
				switch (index)
				{
				case 0:
					return lhs[0] * rhs[0] + lhs[1] * rhs[2];
				case 1:
					return lhs[0] * rhs[1] + lhs[1] * rhs[3];
				case 2:
					return lhs[2] * rhs[0] + lhs[3] * rhs[2];
				case 3:
					return lhs[2] * rhs[1] + lhs[3] * rhs[3];
				case 4:
					return lhs[4] * rhs[0] + lhs[5] * rhs[2] + rhs[4];
				case 5:
					return lhs[4] * rhs[1] + lhs[5] * rhs[3] + rhs[5];
				default:
					return 0.f;
				}
			}
		};

		inline MatrixMultiply<Matrix, Matrix> operator *(Matrix const& lhs, Matrix const& rhs)
		{
			return MatrixMultiply<Matrix, Matrix>(lhs, rhs);
		}

		template <typename L, typename R>
		inline MatrixMultiply<MatrixMultiply<L, R>, Matrix> operator *(MatrixMultiply<L, R> const& lhs, Matrix const& rhs)
		{
			return MatrixMultiply<MatrixMultiply<L, R>, Matrix>(lhs, rhs);
		}
	}
}
