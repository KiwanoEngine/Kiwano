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
#include "Rect.hpp"
#include <d2d1.h>

namespace easy2d
{
	namespace math
	{
		struct Matrix
		{
			union
			{
				struct
				{
					float m[6];  // m[3][2]
				};
				
				struct
				{
					float
						_11, _12,
						_21, _22,
						_31, _32;
				};
			};

			Matrix();

			Matrix(float _11, float _12, float _21, float _22, float _31, float _32);

			Matrix(const float* p);

			Matrix(Matrix const& other);

			template <typename T>
			Matrix(T const& other)
			{
				for (int i = 0; i < 6; i++)
					m[i] = other[i];
			}

			void Identity();

			inline Vector2 Transform(const Vector2& v) const
			{
				return Vector2(
					v.x * _11 + v.y * _21 + _31,
					v.x * _12 + v.y * _22 + _32
				);
			}

			Rect Transform(const Rect& rect) const;

			inline void Matrix::Translate(const Vector2& v)
			{
				_31 += _11 * v.x + _21 * v.y;
				_32 += _12 * v.x + _22 * v.y;
			}

			inline float operator [](unsigned int index) const
			{
				return m[index];
			}

			template <typename T>
			inline Matrix& operator =(T const& other)
			{
				for (int i = 0; i < 6; i++)
					m[i] = other[i];
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

			inline bool IsInvertible() const
			{
				return 0 != Determinant();
			}

			inline operator D2D1_MATRIX_3X2_F const& () const
			{
				return reinterpret_cast<D2D1_MATRIX_3X2_F const&>(*this);
			}

			inline operator D2D1_MATRIX_3X2_F& ()
			{
				return reinterpret_cast<D2D1_MATRIX_3X2_F&>(*this);
			}

			static Matrix Translation(const Vector2& v);

			static Matrix Translation(
				float x,
				float y
			);

			static Matrix Scaling(
				const Vector2& v,
				const Vector2& center = Vector2()
			);

			static Matrix Scaling(
				float x,
				float y,
				const Vector2& center = Vector2()
			);

			static Matrix Rotation(
				float angle,
				const Vector2& center = Vector2()
			);

			static Matrix Skewing(
				float angle_x,
				float angle_y,
				const Vector2& center = Vector2()
			);

			static Matrix Invert(Matrix const& matrix);
		};


		// Use template expression to optimize matrix multiply
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
