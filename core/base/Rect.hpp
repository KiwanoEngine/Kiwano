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
#include "BaseTypes.h"

namespace easy2d
{
	// ����
	//
	// Usage:
	//     ��ʾһ����ά����: Rect rect = Rect(10, 20, 30, 40);  // ���Ͻ�����Ϊ (10, 20), ��Ϊ 30, ��Ϊ 40
	//     ���ο���ͨ�� Point + Size ����, Point ��ʾ�������Ͻ�����, Size ��ʾ���ο��
	//     �ж�һ�����Ƿ��ھ�����: bool contains = rect.ContainsPoint(p);
	//     �ж��������Ƿ��ཻ: bool intersects = rect1.Intersects(rect2);
	//
	class Rect
	{
	public:
		Point origin;	// ���Ͻ�����
		Size  size;		// ��Ⱥ͸߶�

	public:
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
			const Point& pos,
			const Size& size
		)
		: origin(pos.x, pos.y)
		, size(size.width, size.height)
		{}

		Rect(
			const Rect& other
		)
		: origin(other.origin.x, other.origin.y)
		, size(other.size.width, other.size.height)
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

		// �жϵ��Ƿ��ھ�����
		inline bool ContainsPoint(
			const Point& point
		) const
		{
			return	point.x >= origin.x && point.x <= (origin.y + size.height) &&
					point.y >= origin.y && point.y <= (origin.y + size.height);
		}

		// �ж��������Ƿ��ཻ
		inline bool Intersects(
			const Rect& rect
		) const
		{
			return !((origin.x + size.width)			< rect.origin.x ||
					(rect.origin.x + rect.size.width)	< origin.x ||
					(origin.y + size.height)			< rect.origin.y ||
					(rect.origin.y + rect.size.height)	< origin.y);
		}
	};
}
