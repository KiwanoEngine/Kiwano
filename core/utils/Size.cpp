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

e2d::Size::Size()
{
	width = 0;
	height = 0;
}

e2d::Size::Size(float width, float height)
{
	this->width = width;
	this->height = height;
}

e2d::Size::Size(const Size & other)
{
	width = other.width;
	height = other.height;
}

e2d::Size e2d::Size::operator+(const Size & other) const
{
	return Size(width + other.width, height + other.height);
}

e2d::Size e2d::Size::operator-(const Size & other) const
{
	return Size(width - other.width, height - other.height);
}

e2d::Size e2d::Size::operator*(float value) const
{
	return Size(width * value, height * value);
}

e2d::Size e2d::Size::operator/(float value) const
{
	return Size(width / value, height / value);
}

e2d::Size::operator e2d::Point() const
{
	return Point(width, height);
}

e2d::Size e2d::Size::operator-() const
{
	return Size(-width, -height);
}

bool e2d::Size::operator==(const Size & other) const
{
	return (width == other.width) && (height == other.height);
}
