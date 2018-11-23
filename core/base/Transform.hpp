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
#include "Size.hpp"
#include "Point.hpp"
#include "../math/Matrix.hpp"
#include <d2d1.h>

namespace easy2d
{
	class Transform
	{
	public:
		float rotation;		// 旋转
		Point position;		// 坐标
		Point scale;		// 缩放
		Point skew;			// 错切角度

	public:
		Transform()
			: position()
			, rotation(0)
			, scale(1.f, 1.f)
			, skew(0.f, 0.f)
		{}

		bool operator== (const Transform& other) const
		{
			return position == other.position &&
				scale == other.scale &&
				skew == other.skew &&
				rotation == other.rotation;
		}
	};
}
