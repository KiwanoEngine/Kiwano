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
#include "Point.hpp"
#include <d2d1.h>

namespace easy2d
{
    // 大小
	//
	// Usage:
	//     表示一个二维矩形区域的大小: Size s(10, 5);  // 宽为 10, 高为 5
	//     大小可以相加减: Size s = Size(10, 10) + Size(20, 20);  // s 的大小是宽高均为 30
	//
	class Size
	{
	public:
		float width;	// 宽度
		float height;	// 高度

	public:
		Size()
		{
			width = 0;
			height = 0;
		}

		Size(float width, float height)
		{
			this->width = width;
			this->height = height;
		}

		Size(const Size & other)
		{
			width = other.width;
			height = other.height;
		}

		Size(const Point & p)
		{
			width = p.x;
			height = p.y;
		}

		inline const Size operator+(const Size & other) const
		{
			return Size(width + other.width, height + other.height);
		}

		inline const Size operator-(const Size & other) const
		{
			return Size(width - other.width, height - other.height);
		}

		inline const Size operator*(float val) const
		{
			return Size(width * val, height * val);
		}

		inline const Size operator/(float val) const
		{
			return Size(width / val, height / val);
		}

		inline const Size operator-() const
		{
			return Size(-width, -height);
		}

		inline bool operator==(const Size & other) const
		{
			return (width == other.width) && (height == other.height);
		}

		inline operator Point () const
		{
			return Point{ width, height };
		}

		inline operator D2D1_SIZE_F const& () const
		{
			return reinterpret_cast<D2D1_SIZE_F const&>(*this);
		}

		inline operator D2D1_SIZE_F& ()
		{
			return reinterpret_cast<D2D1_SIZE_F&>(*this);
		}
	};
}
