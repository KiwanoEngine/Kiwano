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
#include <d2d1.h>

namespace easy2d
{
	namespace math
	{
		// ����
		struct Rect
		{
			Vector2	origin;	// ���Ͻ�����
			Vector2	size;	// ��Ⱥ͸߶�

			Rect() {}

			Rect(
				float x,
				float y,
				float width,
				float height
			)
				: origin(x, y)
				, size(width, height)
			{}

			Rect(
				const Vector2& pos,
				const Vector2& size
			)
				: origin(pos.x, pos.y)
				, size(size.x, size.y)
			{}

			Rect(
				const Rect& other
			)
				: origin(other.origin.x, other.origin.y)
				, size(other.size.x, other.size.y)
			{}

			Rect(
				const D2D1_RECT_F& other
			)
				: origin(other.left, other.top)
				, size(other.right - other.left, other.bottom - other.top)
			{}

			Rect& operator= (const Rect& other)
			{
				origin = other.origin;
				size = other.size;
				return *this;
			}

			inline bool operator== (const Rect& rect) const
			{
				return (origin == rect.origin) && (size == rect.size);
			}

			inline Vector2 GetCenter() const { return Vector2{ origin.x + size.x / 2, origin.y + size.y / 2 }; }

			inline Vector2 GetLeftTop() const { return origin; }

			inline Vector2 GetRightBottom() const { return Vector2{ GetRight(), GetBottom() }; }

			inline Vector2 GetRightTop() const { return Vector2{ GetRight(), GetTop() }; }

			inline Vector2 GetLeftBottom() const { return Vector2{ GetLeft(), GetBottom() }; }

			inline float GetLeft() const { return origin.x; }

			inline float GetTop() const { return origin.y; }

			inline float GetRight() const { return origin.x + size.x; }

			inline float GetBottom() const { return origin.y + size.y; }

			inline bool IsEmpty() const { return origin.IsOrigin() && size.IsOrigin(); }

			// �жϵ��Ƿ��ھ�����
			inline bool ContainsPoint(const Vector2& point) const
			{
				return	point.x >= origin.x && point.x <= (origin.x + size.x) &&
					point.y >= origin.y && point.y <= (origin.y + size.y);
			}

			// �ж��������Ƿ��ཻ
			inline bool Intersects(const Rect& rect) const
			{
				return !((origin.x + size.x)			< rect.origin.x ||
						(rect.origin.x + rect.size.x)	< origin.x ||
						(origin.y + size.y)				< rect.origin.y ||
						(rect.origin.y + rect.size.y)	< origin.y);
			}

			inline operator D2D1_RECT_F () const
			{
				return D2D1_RECT_F{ origin.x, origin.y, origin.x + size.x, origin.y + size.y };
			}
		};
	}
}
