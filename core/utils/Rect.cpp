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

#include "..\e2dutil.h"

easy2d::Rect::Rect(void)
	: origin()
	, size()
{
}

easy2d::Rect::Rect(float x, float y, float width, float height)
	: origin(x, y)
	, size(width, height)
{
}

easy2d::Rect::Rect(const Point& pos, const Size& size)
	: origin(pos.x, pos.y)
	, size(size.width, size.height)
{
}

easy2d::Rect::Rect(const Rect& other)
	: origin(other.origin.x, other.origin.y)
	, size(other.size.width, other.size.height)
{
}

easy2d::Rect& easy2d::Rect::operator= (const Rect& other)
{
	origin = other.origin;
	size = other.size;
	return *this;
}

bool easy2d::Rect::operator==(const Rect & rect) const
{
	return (origin == rect.origin) && (size == rect.size);
}

bool easy2d::Rect::ContainsPoint(const Point& point) const
{
	if (point.x >= origin.x && point.x <= (origin.y + size.height)
		&& point.y >= origin.y && point.y <= (origin.y + size.height))
	{
		return true;
	}
	return false;
}

bool easy2d::Rect::Intersects(const Rect& rect) const
{
	return !((origin.x + size.width)			< rect.origin.x ||
			(rect.origin.x + rect.size.width)	<	  origin.x ||
			(origin.y + size.height)			< rect.origin.y ||
			(rect.origin.y + rect.size.height)	<	  origin.y);
}
