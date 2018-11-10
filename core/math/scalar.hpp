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
#include <cmath>

namespace easy2d
{
	namespace math
	{
		inline int Abs(int v) { return ::abs(v); }

		inline float Abs(float v) { return ::fabsf(v); }

		inline double Abs(double v) { return ::fabs(v); }

		inline float Sqrt(float v) { return ::sqrtf(v); }

		inline double Sqrt(double v) { return ::sqrt(v); }

		inline int Sign(int v) { return v < 0 ? -1 : 1; }

		inline float Sign(float v) { return v < 0 ? -1.f : 1.f; }

		inline double Sign(double v) { return v < 0 ? -1.0 : 1.0; }

		inline float Sin(float v) { return ::sinf(v); }

		inline double Sin(double v) { return ::sin(v); }

		inline float Cos(float v) { return ::cosf(v); }

		inline double Cos(double v) { return ::cos(v); }

		inline float Tan(float v) { return ::tanf(v); }

		inline double Tan(double v) { return ::tan(v); }

		inline float Ceil(float v) { return ceil(v); }

		inline double Ceil(double v) { return ceil(v); }

		inline float Floor(float v) { return floor(v); }

		inline double Floor(double v) { return floor(v); }
	}
}
