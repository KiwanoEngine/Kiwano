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
		// 矩形
		template <typename _Ty>
		struct RectT
		{
		public:
			using value_type = _Ty;

			Vec2T<value_type> origin;	// 左上角坐标
			Vec2T<value_type> size;	// 宽度和高度

		public:
			RectT() {}

			RectT(
				value_type x,
				value_type y,
				value_type width,
				value_type height
			)
				: origin(x, y)
				, size(width, height)
			{}

			RectT(
				const Vec2& pos,
				const Vec2& size
			)
				: origin(pos.x, pos.y)
				, size(size.x, size.y)
			{}

			RectT(
				const RectT& other
			)
				: origin(other.origin.x, other.origin.y)
				, size(other.size.x, other.size.y)
			{}

			RectT& operator= (const RectT& other)
			{
				origin = other.origin;
				size = other.size;
				return *this;
			}

			inline bool operator== (const RectT& rect) const
			{
				return (origin == rect.origin) && (size == rect.size);
			}

			inline void Set(value_type x, value_type y, value_type width, value_type height) { origin = Vec2{ x, y }; size = Vec2{ width, height }; }

			inline Vec2 GetCenter() const { return Vec2{ origin.x + size.x / 2, origin.y + size.y / 2 }; }

			inline Vec2 GetLeftTop() const { return origin; }

			inline Vec2 GetRightBottom() const { return Vec2{ GetRight(), GetBottom() }; }

			inline Vec2 GetRightTop() const { return Vec2{ GetRight(), GetTop() }; }

			inline Vec2 GetLeftBottom() const { return Vec2{ GetLeft(), GetBottom() }; }

			inline value_type GetLeft() const { return origin.x; }

			inline value_type GetTop() const { return origin.y; }

			inline value_type GetRight() const { return origin.x + size.x; }

			inline value_type GetBottom() const { return origin.y + size.y; }

			inline bool IsEmpty() const { return origin.IsOrigin() && size.IsOrigin(); }

			// 判断点是否在矩形内
			inline bool ContainsPoint(const Vec2& point) const
			{
				return	point.x >= origin.x && point.x <= (origin.x + size.x) &&
					point.y >= origin.y && point.y <= (origin.y + size.y);
			}

			// 判断两矩形是否相交
			inline bool Intersects(const RectT& rect) const
			{
				return !((origin.x + size.x)			< rect.origin.x ||
						(rect.origin.x + rect.size.x)	< origin.x ||
						(origin.y + size.y)				< rect.origin.y ||
						(rect.origin.y + rect.size.y)	< origin.y);
			}
		};
	}
}

namespace kiwano
{
	using Rect = kiwano::math::RectT<float>;
}
