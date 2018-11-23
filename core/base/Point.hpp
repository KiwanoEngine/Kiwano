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
#include "../math/vector.hpp"
#include <d2d1.h>

namespace easy2d
{
	// 坐标
	//
	// Usage:
	//     表示一个二维空间的坐标: Point origin(0, 0);
	//     计算两点间距离: float distance = p1.Distance(p2);
	//     坐标可以相加减: Point p = Point(10, 10) + Point(20, 20);  // p 的坐标是 (30, 30)
	//
	class Point
		: public math::Vector2
	{
	public:
		Point() {}

		Point(float x, float y) : math::Vector2(x, y) {}

		Point(Point const& other) : math::Vector2(other.x, other.y) {}

		inline float Distance(Point const& v)
		{
			return Point(x - v.x, y - v.y).Length();
		}

		inline const Point operator + (const Point & other) const
		{
			return Point(x + other.x, y + other.y);
		}

		inline const Point operator - (const Point & other) const
		{
			return Point(x - other.x, y - other.y);
		}

		inline const Point operator * (float val) const
		{
			return Point(x * val, y * val);
		}

		inline const Point operator / (float val) const
		{
			return Point(x / val, y / val);
		}

		inline const Point operator - () const
		{
			return Point(-x, -y);
		}

		inline bool operator == (const Point& other) const
		{
			return (x == other.x) && (y == other.y);
		}

		inline operator D2D1_POINT_2F const& () const
		{
			return reinterpret_cast<D2D1_POINT_2F const&>(*this);
		}

		inline operator D2D1_POINT_2F& ()
		{
			return reinterpret_cast<D2D1_POINT_2F&>(*this);
		}
	};
}
