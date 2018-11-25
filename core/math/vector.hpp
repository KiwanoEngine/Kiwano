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
#include "scalar.hpp"
#include <d2d1.h>

namespace easy2d
{
	namespace math
	{
		class Vector2
		{
		public:
			float x;
			float y;

		public:
			Vector2() : x(0.f), y(0.f) {}

			Vector2(float x, float y) : x(x), y(y) {}

			Vector2(const Vector2& other) : x(other.x), y(other.y) {}

			inline float Length() const
			{
				return math::Sqrt(x * x + y * y);
			}

			inline const Vector2 operator + (const Vector2 & other) const
			{
				return Vector2(x + other.x, y + other.y);
			}

			inline const Vector2 operator - (const Vector2 & other) const
			{
				return Vector2(x - other.x, y - other.y);
			}

			inline const Vector2 operator * (float val) const
			{
				return Vector2(x * val, y * val);
			}

			inline const Vector2 operator / (float val) const
			{
				return Vector2(x / val, y / val);
			}

			inline const Vector2 operator - () const
			{
				return Vector2(-x, -y);
			}

			inline bool operator== (const Vector2& other) const
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
}