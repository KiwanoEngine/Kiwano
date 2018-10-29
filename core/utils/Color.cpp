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

static const UINT kRedShift = 16;
static const UINT kGreenShift = 8;
static const UINT kBlueShift = 0;

static const UINT kRedMask = 0xff << kRedShift;
static const UINT kGreenMask = 0xff << kGreenShift;
static const UINT kBlueMask = 0xff << kBlueShift;

easy2d::Color::Color()
	: r(0)
	, g(0)
	, b(0)
	, a(1.f)
{
}

easy2d::Color::Color(float r, float g, float b)
	: r(r)
	, g(g)
	, b(b)
	, a(1.f)
{
}

easy2d::Color::Color(float r, float g, float b, float alpha)
	: r(r)
	, g(g)
	, b(b)
	, a(alpha)
{
}

easy2d::Color::Color(UINT rgb)
	: r(((rgb & kRedMask) >> kRedShift) / 255.f)
	, g(((rgb & kGreenMask) >> kGreenShift) / 255.f)
	, b(((rgb & kBlueMask) >> kBlueShift) / 255.f)
	, a(1.f)
{
}

easy2d::Color::Color(UINT rgb, float alpha)
	: r(((rgb & kRedMask) >> kRedShift) / 255.f)
	, g(((rgb & kGreenMask) >> kGreenShift) / 255.f)
	, b(((rgb & kBlueMask) >> kBlueShift) / 255.f)
	, a(alpha)
{
}

easy2d::Color::Color(const D2D1_COLOR_F& color)
	: r(color.r)
	, g(color.g)
	, b(color.b)
	, a(color.a)
{
}

easy2d::Color::operator D2D1_COLOR_F() const
{
	return D2D1::ColorF(r, g, b, a);
}
