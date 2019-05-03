// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include "Vec2.hpp"
#include "Rect.hpp"
#include <algorithm>

namespace kiwano
{
	namespace math
	{
		template <typename _Ty, typename _Lty, typename _Rty>
		struct MatrixMultiply;

		template <typename _Ty>
		struct MatrixT
		{
			using value_type = _Ty;

			union
			{
				struct
				{
					_Ty m[6];  // m[3][2]
				};
				
				struct
				{
					_Ty
						_11, _12,
						_21, _22,
						_31, _32;
				};
			};

			MatrixT()
				: _11(1.f), _12(0.f)
				, _21(0.f), _22(1.f)
				, _31(0.f), _32(0.f)
			{
			}

			MatrixT(value_type _11, value_type _12, value_type _21, value_type _22, value_type _31, value_type _32)
				: _11(_11), _12(_12), _21(_21), _22(_22), _31(_31), _32(_32)
			{
			}

			explicit MatrixT(const value_type* p)
			{
				for (int i = 0; i < 6; i++)
					m[i] = p[i];
			}

			MatrixT(MatrixT const& other)
				: _11(other._11), _12(other._12)
				, _21(other._21), _22(other._22)
				, _31(other._31), _32(other._32)
			{
			}

			template <typename T>
			MatrixT(T const& other)
			{
				for (int i = 0; i < 6; i++)
					m[i] = other[i];
			}

			inline void Identity()
			{
				_11 = 1.f; _12 = 0.f;
				_21 = 0.f; _12 = 1.f;
				_31 = 0.f; _32 = 0.f;
			}

			inline Vec2 Transform(const Vec2& v) const
			{
				return Vec2(
					v.x * _11 + v.y * _21 + _31,
					v.x * _12 + v.y * _22 + _32
				);
			}

			RectT<value_type> Transform(const Rect & rect) const
			{
				Vec2T<value_type> top_left = Transform(rect.GetLeftTop());
				Vec2T<value_type> top_right = Transform(rect.GetRightTop());
				Vec2T<value_type> bottom_left = Transform(rect.GetLeftBottom());
				Vec2T<value_type> bottom_right = Transform(rect.GetRightBottom());

				value_type left = std::min(std::min(top_left.x, top_right.x), std::min(bottom_left.x, bottom_right.x));
				value_type right = std::max(std::max(top_left.x, top_right.x), std::max(bottom_left.x, bottom_right.x));
				value_type top = std::min(std::min(top_left.y, top_right.y), std::min(bottom_left.y, bottom_right.y));
				value_type bottom = std::max(std::max(top_left.y, top_right.y), std::max(bottom_left.y, bottom_right.y));

				return Rect{ left, top, (right - left), (bottom - top) };
			}

			inline void Translate(const Vec2& v)
			{
				_31 += _11 * v.x + _21 * v.y;
				_32 += _12 * v.x + _22 * v.y;
			}

			inline value_type operator [](unsigned int index) const
			{
				return m[index];
			}

			template <typename _Lty, typename _Rty>
			inline MatrixT& operator= (MatrixMultiply<value_type, _Lty, _Rty> const& other)
			{
				for (int i = 0; i < 6; i++)
					m[i] = other[i];
				return *this;
			}

			inline value_type Determinant() const
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

			static inline MatrixT Translation(const Vec2& v)
			{
				return MatrixT(
					1.f, 0.f,
					0.f, 1.f,
					v.x, v.y
				);
			}

			static inline MatrixT Translation(
				value_type x,
				value_type y)
			{
				return Translation(Vec2(x, y));
			}

			static inline MatrixT Scaling(
				const Vec2& v,
				const Vec2& center = Vec2())
			{
				return MatrixT(
					v.x, 0.f,
					0.f, v.y,
					center.x - v.x * center.x,
					center.y - v.y * center.y
				);
			}

			static inline MatrixT Scaling(
				value_type x,
				value_type y,
				const Vec2& center = Vec2())
			{
				return Scaling(Vec2(x, y), center);
			}

			static inline MatrixT Rotation(
				value_type angle,
				const Vec2& center = Vec2())
			{
				value_type s = math::Sin(angle);
				value_type c = math::Cos(angle);
				return MatrixT(
					c, s,
					-s, c,
					center.x * (1 - c) + center.y * s,
					center.y * (1 - c) - center.x * s
				);
			}

			static inline MatrixT Skewing(
				value_type angle_x,
				value_type angle_y,
				const Vec2& center = Vec2())
			{
				value_type tx = math::Tan(angle_x);
				value_type ty = math::Tan(angle_y);
				return MatrixT(
					1.f, -ty,
					-tx, 1.f,
					center.y * tx, center.x * ty
				);
			}

			static inline MatrixT Invert(MatrixT const& matrix)
			{
				value_type det = 1.f / matrix.Determinant();

				return MatrixT(
					det * matrix._22,
					-det * matrix._12,
					-det * matrix._21,
					det * matrix._11,
					det * (matrix._21 * matrix._32 - matrix._22 * matrix._31),
					det * (matrix._12 * matrix._31 - matrix._11 * matrix._32)
				);
			}
		};


		// Use template expression to optimize matrix multiply
		template <typename _Ty, typename _Lty, typename _Rty>
		struct MatrixMultiply
		{
			_Lty const& lhs;
			_Rty const& rhs;

			MatrixMultiply(_Lty const& lhs, _Rty const& rhs)
				: lhs(lhs)
				, rhs(rhs)
			{}

			inline _Ty operator [](unsigned int index) const
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

		template <typename _Ty>
		inline
		MatrixMultiply<_Ty, MatrixT<_Ty>, MatrixT<_Ty>>
		operator *(MatrixT<_Ty> const& lhs, MatrixT<_Ty> const& rhs)
		{
			return MatrixMultiply<_Ty, MatrixT<_Ty>, MatrixT<_Ty>>(lhs, rhs);
		}

		template <typename _Ty, typename _Lty, typename _Rty>
		inline
		MatrixMultiply<_Ty, MatrixMultiply<_Ty, _Lty, _Rty>, MatrixT<_Ty>>
		operator *(MatrixMultiply<_Ty, _Lty, _Rty> const& lhs, MatrixT<_Ty> const& rhs)
		{
			return MatrixMultiply<_Ty, MatrixMultiply<_Ty, _Lty, _Rty>, MatrixT<_Ty>>(lhs, rhs);
		}
	}
}

namespace kiwano
{
	using Matrix = kiwano::math::MatrixT<float>;
}
