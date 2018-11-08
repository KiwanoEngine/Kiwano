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
#include "../base/BaseTypes.h"
#include "../base/Size.h"
#include <d2d1.h>

namespace easy2d
{
	namespace math
	{
		class Transform
		{
		public:
			Point position;		// 坐标
			Size size;			// 大小
			float scale_x;		// 横向缩放
			float scale_y;		// 纵向缩放
			float rotation;		// 旋转
			float skew_x;		// 横向倾斜角度
			float skew_y;		// 纵向倾斜角度
			float pivot_x;		// 支点横坐标
			float pivot_y;		// 支点纵坐标

		public:
			Transform();

			explicit operator D2D1::Matrix3x2F() const;

			bool operator== (const Transform& other) const;
		};
	}
}
