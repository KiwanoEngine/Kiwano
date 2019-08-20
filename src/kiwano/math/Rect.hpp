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

namespace kiwano
{
	namespace math
	{
		template <typename _Ty>
		struct RectT
		{
		public:
			using value_type = _Ty;

			Vec2T<value_type> left_top;
			Vec2T<value_type> right_bottom;

		public:
			RectT() {}

			RectT(
				value_type left,
				value_type top,
				value_type right,
				value_type bottom
			)
				: left_top(left, top)
				, right_bottom(right, bottom)
			{}

			RectT(
				const Vec2T<value_type>& left_top,
				const Vec2T<value_type>& right_bottom
			)
				: left_top(left_top)
				, right_bottom(right_bottom)
			{}

			RectT(
				const RectT& other
			)
				: left_top(other.left_top)
				, right_bottom(other.right_bottom)
			{}

			RectT& operator= (const RectT& other)
			{
				left_top = other.left_top;
				right_bottom = other.right_bottom;
				return *this;
			}

			inline bool operator== (const RectT& rect) const
			{
				return (left_top == rect.left_top) && (right_bottom == rect.right_bottom);
			}

			inline void Set(value_type left, value_type top, value_type right, value_type bottom)
			{
				left_top = Vec2T<value_type>{ left, top };
				right_bottom = Vec2T<value_type>{ right, bottom };
			}

			inline Vec2T<value_type> GetCenter() const		{ return Vec2T<value_type>{ (left_top.x + right_bottom.x) / 2, (left_top.y + right_bottom.y) / 2 }; }

			inline Vec2T<value_type> GetLeftTop() const		{ return left_top; }

			inline Vec2T<value_type> GetRightBottom() const	{ return right_bottom; }

			inline Vec2T<value_type> GetRightTop() const	{ return Vec2T<value_type>{ right_bottom.x, left_top.y }; }

			inline Vec2T<value_type> GetLeftBottom() const	{ return Vec2T<value_type>{ left_top.x, right_bottom.y }; }

			inline value_type GetLeft() const				{ return left_top.x; }

			inline value_type GetTop() const				{ return left_top.y; }

			inline value_type GetRight() const				{ return right_bottom.x; }

			inline value_type GetBottom() const				{ return right_bottom.y; }

			inline value_type GetWidth() const				{ return right_bottom.x - left_top.x; }

			inline value_type GetHeight() const				{ return right_bottom.y - left_top.y; }

			inline Vec2T<value_type> GetSize() const		{ return Vec2T<value_type>{ GetWidth(), GetHeight() }; }

			inline bool IsEmpty() const						{ return left_top.IsOrigin() && right_bottom.IsOrigin(); }

			inline bool ContainsPoint(const Vec2T<value_type>& point) const
			{
				return	point.x >= left_top.x && point.x <= right_bottom.x &&
					point.y >= left_top.y && point.y <= right_bottom.y;
			}

			inline bool Intersects(const RectT& rect) const
			{
				return !(right_bottom.x		< rect.left_top.x ||
						rect.right_bottom.x	< left_top.x ||
						right_bottom.y		< rect.left_top.y ||
						rect.right_bottom.y	< left_top.y);
			}

			static inline RectT Infinite()
			{
				return RectT{ -math::constants::FLOAT_MAX, -math::constants::FLOAT_MAX, math::constants::FLOAT_MAX, math::constants::FLOAT_MAX };
			}
		};
	}
}
