// Copyright (c) 2016-2018 Kiwano - Nomango
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

#include "Color.h"

namespace kiwano
{
	namespace
	{
		const UInt32 RED_SHIFT = 16;
		const UInt32 GREEN_SHIFT = 8;
		const UInt32 BLUE_SHIFT = 0;

		const UInt32 RED_MASK = 0xff << RED_SHIFT;
		const UInt32 GREEN_MASK = 0xff << GREEN_SHIFT;
		const UInt32 BLUE_MASK = 0xff << BLUE_SHIFT;
	}

	Color::Color()
		: r(0)
		, g(0)
		, b(0)
		, a(1.f)
	{
	}

	Color::Color(Float32 r, Float32 g, Float32 b)
		: r(r)
		, g(g)
		, b(b)
		, a(1.f)
	{
	}

	Color::Color(Float32 r, Float32 g, Float32 b, Float32 alpha)
		: r(r)
		, g(g)
		, b(b)
		, a(alpha)
	{
	}

	Color::Color(UInt32 rgb)
		: r(((rgb & RED_MASK) >> RED_SHIFT) / 255.f)
		, g(((rgb & GREEN_MASK) >> GREEN_SHIFT) / 255.f)
		, b(((rgb & BLUE_MASK) >> BLUE_SHIFT) / 255.f)
		, a(1.f)
	{
	}

	Color::Color(UInt32 rgb, Float32 alpha)
		: r(((rgb & RED_MASK) >> RED_SHIFT) / 255.f)
		, g(((rgb & GREEN_MASK) >> GREEN_SHIFT) / 255.f)
		, b(((rgb & BLUE_MASK) >> BLUE_SHIFT) / 255.f)
		, a(alpha)
	{
	}
}