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

#if _MSC_VER >= 1900
#	define E2D_CONSTEXPR constexpr
#else
#	define E2D_CONSTEXPR const
#endif

namespace easy2d
{
	namespace math
	{
		namespace constants
		{
			E2D_CONSTEXPR auto PI_F     = 3.141592653589793f;
			E2D_CONSTEXPR auto PI_F_2   = 1.570796326794896f;
			E2D_CONSTEXPR auto PI_F_X_2 = 6.283185307179586f;

			E2D_CONSTEXPR auto PI_D     = 3.14159265358979323846;
			E2D_CONSTEXPR auto PI_D_2   = 1.57079632679489661923;
			E2D_CONSTEXPR auto PI_D_X_2 = 6.28318530717958647692;
		}
	}
}
