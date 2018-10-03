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
#include <cmath>


e2d::Point::Point()
{
	x = 0;
	y = 0;
}

e2d::Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

e2d::Point::Point(const Point & other)
{
	x = other.x;
	y = other.y;
}

e2d::Point e2d::Point::operator+(const Point & p) const
{
	return Point(x + p.x, y + p.y);
}

e2d::Point e2d::Point::operator-(const Point & p) const
{
	return Point(x - p.x, y - p.y);
}

e2d::Point e2d::Point::operator*(float value) const
{
	return Point(x * value, y * value);
}

e2d::Point e2d::Point::operator/(float value) const
{
	return Point(x / value, y / value);
}

e2d::Point::operator e2d::Size() const
{
	return Size(x, y);
}

float e2d::Point::Distance(const Point &p1, const Point &p2)
{
	return sqrt(
		(p1.x - p2.x) * (p1.x - p2.x) + 
		(p1.y - p2.y) * (p1.y - p2.y)
	);
}

e2d::Point e2d::Point::operator-() const
{
	return Point(-x, -y);
}

bool e2d::Point::operator==(const Point & point) const
{
	return (x == point.x) && (y == point.y);
}
